# @TEST-EXEC: bro -NN Demo::AllCaps |sed -e 's/version.*)/version)/g' >output
# @TEST-EXEC: btest-diff output
