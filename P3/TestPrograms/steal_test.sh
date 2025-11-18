# This child process will try to read from stdin.
# If your shell is correct, it will get /dev/null.
TestPrograms/test_recieve

# If the test_receiver didn't "steal" this line,
# your shell will read and execute this second.
TestPrograms/test_ping