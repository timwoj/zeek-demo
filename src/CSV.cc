// See the file "COPYING" in the main distribution directory for copyright.

#include <string>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "CSV.h"

using namespace logging::writer;
using namespace threading;
using threading::Value;
using threading::Field;

CSV::CSV(WriterFrontend* frontend) : WriterBackend(frontend)
	{
       	// Use the default "Bro logs" format.
       	desc.EnableEscaping();
       	desc.AddEscapeSequence(",");
       	formatter::Ascii::SeparatorInfo sep_info(",", ",", "", "");
	formatter = new formatter::Ascii(this, sep_info);
	}

CSV::~CSV()
	{
	if ( ! csv_done )
		// In case of errors aborting the logging altogether,
		// DoFinish() may not have been called.
		CloseFile();

	delete formatter;
	}

void CSV::CloseFile()
	{
	if ( ! fd )
		return;

	safe_close(fd);
	fd = 0;
	}

bool CSV::DoInit(const WriterInfo& info, int num_fields, const Field* const * fields)
	{
	assert(! fd);

	string path = info.path;

	fname = path + "." + LogExt();
	fd = open(fname.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);

	if ( fd < 0 )
		{
		Error(Fmt("cannot open %s: %s", fname.c_str(),
			  Strerror(errno)));
		fd = 0;
		return false;
		}

	if ( ! WriteHeader(path) )
		{
		Error(Fmt("error writing to %s: %s", fname.c_str(), Strerror(errno)));
		return false;
		}

	return true;
	}

bool CSV::WriteHeader(const string& path)
	{
	string names;

	for ( int i = 0; i < NumFields(); ++i )
		{
		if ( i > 0 )
			names += ",";

		names += string(Fields()[i]->name);
		}

	// A single line is all we need
	string str = names + "\n";
	if ( ! safe_write(fd, str.c_str(), str.length()) )
		return false;

	return true;
	}

bool CSV::DoFlush(double network_time)
	{
	fsync(fd);
	return true;
	}

bool CSV::DoFinish(double network_time)
	{
	if ( csv_done )
		{
		fprintf(stderr, "internal error: duplicate finish\n");
		abort();
		}

	DoFlush(network_time);

	csv_done = true;

	CloseFile();

	return true;
	}

bool CSV::DoWrite(int num_fields, const Field* const * fields,
		  Value** vals)
	{
	if ( ! fd )
		DoInit(Info(), NumFields(), Fields());

	desc.Clear();

	if ( ! formatter->Describe(&desc, num_fields, fields, vals) )
		return false;

	desc.AddRaw("\n", 1);

	const char* bytes = (const char*)desc.Bytes();
	int len = desc.Len();

	if ( ! safe_write(fd, bytes, len) )
		{
		Error(Fmt("error writing to %s: %s", fname.c_str(), Strerror(errno)));
		return false;
		}

	if ( ! IsBuf() )
		fsync(fd);

	return true;
	}

bool CSV::DoRotate(const char* rotated_path, double open, double close, bool terminating)
	{
	// Don't rotate special files or if there's not one currently open.
	if ( ! fd )
		{
		FinishedRotation();
		return true;
		}

	CloseFile();

	string nname = string(rotated_path) + "." + LogExt();

	if ( rename(fname.c_str(), nname.c_str()) != 0 )
		{
		char buf[256];
		bro_strerror_r(errno, buf, sizeof(buf));
		Error(Fmt("failed to rename %s to %s: %s", fname.c_str(),
			  nname.c_str(), buf));
		FinishedRotation();
		return false;
		}

	if ( ! FinishedRotation(nname.c_str(), fname.c_str(), open, close, terminating) )
		{
		Error(Fmt("error rotating %s to %s", fname.c_str(), nname.c_str()));
		return false;
		}

	return true;
	}

bool CSV::DoSetBuf(bool enabled)
	{
	// Nothing to do.
	return true;
	}

bool CSV::DoHeartbeat(double network_time, double current_time)
	{
	// Nothing to do.
	return true;
	}

string CSV::LogExt()
	{
	return "csv";
	}
