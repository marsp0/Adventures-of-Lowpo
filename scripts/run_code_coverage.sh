#!/bin/bash
make gcov && ./out/gcov_exe
chmod +x ./scripts/get_code_coverage_reports.sh
cd ./out
../scripts/get_code_coverage_reports.sh
cd ../
codecov