#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}
@test "Check built-in cd runs without errors" {
    run ./dsh <<EOF                
cd ../
EOF

    # Assertions
    [ "$status" -eq 0 ]
}
@test "Check built-in exit runs without errors" {
    run ./dsh <<EOF                
cd ../
EOF

    # Assertions
    [ "$status" -eq 0 ]
}
@test "Check built-in dragon works with pipes" {
    run ./dsh <<EOF                
dragon | wc -l
EOF

    # Assertions# Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="38dsh3>dsh3>cmdloopreturned0"

    
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}
@test "Check no commands works" {
    run ./dsh <<EOF                
EOF

    # Assertions# Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh3>cmdloopreturned0"

    
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}
@test "Check 8+ commands fails" {
    
    run ./dsh <<EOF 
    dragon | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l         
EOF

    # Assertions# Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]'| cut -c7-)

    # Expected output with all whitespace removed for easier matching
    expected_output="error:pipinglimitedto8commandscmdloopreturned-2"

    
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}
@test "Check mutiple pipes passes (<8)" {
    run ./dsh <<EOF 
    dragon | wc -l | wc -l | wc -l          
EOF

    # Assertions# Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="1dsh3>dsh3>cmdloopreturned0"

    
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}
@test "Check bad args fail (<8)" {
    run ./dsh <<EOF 
    dragon | wc -l | wc -q         
EOF

    # Assertions# Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="wc:invalidoption--'q'Try'wc--help'formoreinformation.dsh3>dsh3>cmdloopreturned0"

    
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}
@test "Check mutiple commands with pipes is fine" {
    run ./dsh <<EOF 
    dragon | wc 
    dragon | wc -l 
    dragon | wc -c       
EOF

    # Assertions# Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="381293842383854dsh3>dsh3>dsh3>dsh3>cmdloopreturned0"

    
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}
@test "Can pipe text" {
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
    expected_output="Ilovecats12dsh3>dsh3>dsh3>dsh3>cmdloopreturned0"

    
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


