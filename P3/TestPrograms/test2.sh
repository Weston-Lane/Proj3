################################################################
echo "--- TEST: status + conditionals ---"

# AND after success should run
TestPrograms/test_status 0
and echo AND_SHOULD_RUN_AFTER_STATUS_0

# AND after failure should NOT run
TestPrograms/test_status 7
and echo AND_SHOULD_NOT_RUN_AFTER_STATUS_7

# OR after success should NOT run
TestPrograms/test_status 0
or echo OR_SHOULD_NOT_RUN_AFTER_STATUS_0

# OR after failure should run
TestPrograms/test_status 9
or echo OR_SHOULD_RUN_AFTER_STATUS_9

################################################################
echo "--- TEST: pipelines + conditionals ---"

# Both succeed, last=0 -> AND should run
TestPrograms/test_status 0 | TestPrograms/test_status 0
and echo PIPE_AND_SHOULD_RUN_LAST0

# First fails, last succeeds -> pipeline still success, AND should run
TestPrograms/test_status 3 | TestPrograms/test_status 0
and echo PIPE_AND_SHOULD_RUN_LAST0_NONZERO_FIRST

# Last fails -> AND should NOT run
TestPrograms/test_status 0 | TestPrograms/test_status 5
and echo PIPE_AND_SHOULD_NOT_RUN_LAST5

# Last fails -> OR should run
TestPrograms/test_status 0 | TestPrograms/test_status 5
or echo PIPE_OR_SHOULD_RUN_LAST5

# 3-stage pipeline, all succeed -> AND should run
TestPrograms/test_ping | TestPrograms/test_cat | TestPrograms/test_recieve
and echo TEST2_PIPE3_ALL_OK_AND_SHOULD_RUN

# First fails, last succeeds -> pipeline still success (last=0) -> AND should run
TestPrograms/test_status 9 | TestPrograms/test_status 0 | TestPrograms/test_cat
and echo TEST22_PIPE_STATUS9_THEN0_AND_SHOULD_RUN

# Last fails -> AND should NOT run, OR should run
TestPrograms/test_status 0 | TestPrograms/test_status 4
and echo TEST2_PIPE_LAST4_AND_SHOULD_NOT_RUN
or echo TEST2_PIPE_LAST4_OR_SHOULD_RUN

################################################################
echo "--- TEST: redirection + builtins ---"

# pwd to file, then feed that file into a program via stdin
pwd > TestPrograms/test2_pwd.txt
echo EXPECT_PWD_BELOW
TestPrograms/test_recieve < TestPrograms/test2_pwd.txt

# pwd into file, then cat it through a pipeline
pwd > TestPrograms/test2_pwd.txt
TestPrograms/test_cat < TestPrograms/test2_pwd.txt | TestPrograms/test_recieve

# which echo to file, then read it back
which echo > TestPrograms/test2_which_echo.txt
echo EXPECT_WHICH_ECHO_BELOW
TestPrograms/test_recieve < TestPrograms/test2_which_echo.txt

# which echo into file, then use pipeline to read it back
which echo > TestPrograms/test2_which_echo.txt
TestPrograms/test_cat < TestPrograms/test2_which_echo.txt | TestPrograms/test_recieve

# cd into TestPrograms, run ./test_ping, then cd back
cd TestPrograms
./test_ping
cd ../
pwd
cd P3
# Failing cd should flip status for conditionals
cd /this/does/not/exist
and echo TEST27_CD_BAD_AND_SHOULD_NOT_RUN
or echo TEST27_CD_BAD_OR_SHOULD_RUN

################################################################
echo "--- TEST: stdin behavior ---"

# No redirection: test_stdin should usually say "no input" in batch
#TestPrograms/test_stdin

# With input redirection: should read the first line of input.txt
TestPrograms/test_stdin < TestPrograms/input.txt

# Pipe into test_stdin: stdin should come from previous program
echo PIPELINE_INPUT_LINE | TestPrograms/test_stdin

################################################################
echo "--- TEST: stdout vs stderr redirection ---"

# Only STDOUT should go to out_only.txt; STDERR stays on terminal
TestPrograms/test_out_err > TestPrograms/out_only.txt

# Show what went into out_only.txt
echo EXPECT_ONLY_STDOUT_IN_FILE_BELOW
TestPrograms/test_recieve < TestPrograms/out_only.txt

################################################################
echo "--- TEST: error handling ---"

# Non-existent command
no_such_command_should_fail
and echo SHOULD_NOT_RUN_AFTER_BAD_COMMAND

# Missing input file for redirection
TestPrograms/test_recieve < TestPrograms/does_not_exist_xyz.txt
and echo SHOULD_NOT_RUN_AFTER_BAD_REDIRECT
