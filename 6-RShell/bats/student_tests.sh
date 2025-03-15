#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file
setup(){
    ./dsh -s &
    sleep 2
    SERVER_PID=$!
}
teardown(){
    if ps -p "$SERVER_PID" > /dev/null; then
        echo "Stopping server (PID: $SERVER_PID)..."
        kill -9 "$SERVER_PID"
        sleep 1  
        if ps -p "$SERVER_PID" > /dev/null; then
            kill -9 "$SERVER_PID"
        fi
    else
        echo "Server process not found, might have exited already."
    fi
    echo "" > server_log
}

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}
@test "Stop-server works" {
    
    run ./dsh -c <<EOF
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]\004')

    # Expected output with all whitespace removed for easier matching
    expected="socketclientmode:addr:127.0.0.1:1234dsh4>clientrequestedservertostop,stopping...cmdloopreturned0"
    expected_output=$(echo "$expected" | tr -d '[:space:]')
    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} = ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}
@test "Pipes work in rshell" {
    
    run ./dsh -c <<EOF
ls -l | wc -l
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]\004' )

    # Expected output with all whitespace removed for easier matching
    expected="socketclientmode:addr:127.0.0.1:1234dsh4>14dsh4>cmdloopreturned0"
    expected_output=$(echo "$expected" | tr -d '[:space:]')

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}
@test "Lots of pipes" {
    
    run ./dsh -c <<EOF
cat dshlib.c | wc -l | wc -c | wc -l
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]\004' )

    # Expected output with all whitespace removed for easier matching
    expected="socketclientmode:addr:127.0.0.1:1234dsh4>1dsh4>cmdloopreturned0"
    expected_output=$(echo "$expected" | tr -d '[:space:]')

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}
@test "establish connection with server" {
    ./dsh -s > server_log

    expected="
   socket server mode:  addr:0.0.0.0:1234
   -> Single-Threaded Mode
   cmd loop returned -50"
    expected_output=$(echo "$expected" | tr -d '[:space:]')
    run grep "$expected" server_log
    
    

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]\004' )

    # Expected output with all whitespace removed for easier matching
    expected="
   socket server mode:  addr:0.0.0.0:1234
   -> Single-Threaded Mode
   cmd loop returned -50"
    expected_output=$(echo "$expected" | tr -d '[:space:]')

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    
    # Check exact match
    #[ "$stripped_output" = "$expected_output" ]

    # Assertions
     [ "$status" -eq 0 ]
}
@test "server log ls" {
    run ./dsh -s > server_log & 
    run ./dsh -c <<EOF
ls -l
EOF

    expected="
   socket server mode:  addr:0.0.0.0:1234
-> Single-Threaded Mode
Client Connected.
rdsh-exec:  ls -l
rdsh-exec:  rc = 0"
    expected_output=$(echo "$expected" | tr -d '[:space:]')
    

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]\004' )
    echo "$expected" > server_log
    
    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    
    # Check exact match
    #[ "$stripped_output" = "$expected_output" ]
    run grep "$expected" server_log
    # Assertions
     [ "$status" -eq 0 ]
}
#local mode tests work
@test "Check mutiple commands with pipes is fine" {
    run ./dsh <<EOF 
    dragon | wc 
    dragon | wc -l 
    dragon | wc -c       
EOF

    # Assertions# Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="391313853393865localmodedsh4>dsh4>dsh4>dsh4>cmdloopreturned0"

    
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}
@test "Can pipe text around" {
    current=$(pwd)
    cd /tmp
    echo "I love cats" > cat.txt 
    run "${current}/dsh" <<EOF 
    cd /tmp
    cat cat.txt
    cat cat.txt | wc -c 
EOF

    # Assertions# Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="Ilovecats12localmodedsh4>dsh4>dsh4>dsh4>cmdloopreturned0"

    
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}
@test "2 lines of commands in rshell" {
    
    run ./dsh -c <<EOF
ls -l | wc -l
cat dshlib.h | wc -c 
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]\004' )

    # Expected output with all whitespace removed for easier matching
    expected="socketclientmode:addr:127.0.0.1:1234dsh4>14dsh4>2460dsh4>cmdloopreturned0"
    expected_output=$(echo "$expected" | tr -d '[:space:]')

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}
@test "cd in rshell" {
    expected=$(ls ../)
    run ./dsh -c <<EOF
cd ../
ls
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]\004' )

    # Expected output with all whitespace removed for easier matching
    expected_strip=$(echo "$expected" | tr -d '[:space:]')
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>dsh4>${expected_strip}dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}
