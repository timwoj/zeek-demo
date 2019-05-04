// See the file "COPYING" in the main distribution directory for copyright.
//
// Log writer for delimiter-separated ASCII logs.
#ifndef BRO_PLUGIN_DEMO_ALLCAPS_CSV_H
#define BRO_PLUGIN_DEMO_ALLCAPS_CSV_H

#include "logging/WriterBackend.h"
#include "threading/formatters/Ascii.h"

namespace logging {
namespace writer {

class CSV : public WriterBackend {
public:
	explicit CSV(WriterFrontend* frontend);
	~CSV() override;

	static string LogExt();

	static WriterBackend* Instantiate(WriterFrontend* frontend)
		{ return new CSV(frontend); }

protected:
	bool DoInit(const WriterInfo& info, int num_fields,
			    const threading::Field* const* fields) override;
	bool DoWrite(int num_fields, const threading::Field* const* fields,
			     threading::Value** vals) override;
	bool DoSetBuf(bool enabled) override;
	bool DoRotate(const char* rotated_path, double open,
			      double close, bool terminating) override;
	bool DoFlush(double network_time) override;
	bool DoFinish(double network_time) override;
	bool DoHeartbeat(double network_time, double current_time) override;

private:
	bool WriteHeader(const string& path);
	void CloseFile();
  
	int fd;
	string fname;
	ODesc desc;
	bool csv_done;
	threading::formatter::Formatter* formatter;
};

}
}


#endif
