############################################################################################################
echo "--- TEST: Single Program ---"

TestPrograms/test_ping
############################################################################################################
echo "--- TEST: Pipe ---"

TestPrograms/test_ping | TestPrograms/test_recieve
############################################################################################################
echo "--- TEST: Input Redirection ---"

TestPrograms/test_recieve < TestPrograms/input.txt
############################################################################################################
echo "--- TEST: Multi ARG file ---"
TestPrograms/test_recieveArgs hello I am arg
############################################################################################################
echo "--- TEST: Multi ARG file pipe---"
TestPrograms/test_recieveArgs hello I am arg | TestPrograms/test_recieve
############################################################################################################
echo "--- TEST: Triple pipe ---"
TestPrograms/test_ping | TestPrograms/test_recieve | TestPrograms/test_recieve
############################################################################################################
echo "--- TEST: Multi Redirect ---"
echo sending input.txt to combinedout.txt
TestPrograms/test_recieve < TestPrograms/input.txt > TestPrograms/combined_out.txt
############################################################################################################
echo "--- TEST: AND, OR keyword ---"

echo SHOULD PRINT NOTHING:
TestPrograms/test_succeed
or TestPrograms/test_ping
echo SHOULD PRINT PINGER:
TestPrograms/test_succeed
and TestPrograms/test_ping
############################################################################################################
echo "--- TEST: Fail, AND, OR keyword ---"
#echo is considered a succesful command so be careful
echo SHOULD PRINT NOTHING:
TestPrograms/test_fail
and TestPrograms/test_ping
echo SHOULD PRINT PINGER:
TestPrograms/test_fail
or TestPrograms/test_ping

############################################################################################################
echo "--- TEST: keyword file redirection ---"

echo "File out pwd"
pwd > TestPrograms/pwd.txt
echo "File out which"
which echo > TestPrograms/which.txt
############################################################################################################
echo "--- TEST: OR keyword ---"

echo SHOULD PRINT NOTHING:
TestPrograms/test_succeed
or echo Test Success!
echo SHOULD PRINT MESSAGE:
TestPrograms/test_fail
or echo Test FAILED!

echo "--- TEST: dev/null pipe ---"


TestPrograms/test_recieve

TestPrograms/test_ping
############################################################################################################
echo "--- TEST: DIE and EXIT keyword ---"

#die I have died > TestPrograms/die.txt
exit > TestPrograms/exit.txt

############################################################################################################