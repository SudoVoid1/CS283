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
@test "cd to non-existent dir gives error" {
    run ./dsh <<EOF
cd non_existent_dir 
EOF
    
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>cmdloopreturned-4"
    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]

}
@test "dragon" {
    orginal=$pwd
    run ./dsh <<EOF
dragon
EOF
    
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    echo $stripped_output
    expected_output="dsh2>@%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%@%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%@%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%@%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%@%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%@%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%@dsh2>cmdloopreturned0"
    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]

}
@test "exit works" {
    run ./dsh <<EOF
exit
EOF
    
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>"
    [ "$stripped_output" = "$expected_output" ]
}
@test "cd up in the tree" {
    run ./dsh <<EOF
cd ../
pwd
cd ../
pwd
cd ../
pwd
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="/home/lucas/CS283/home/lucas/homedsh2>dsh2>dsh2>dsh2>dsh2>dsh2>dsh2>cmdloopreturned0"
    [ "$stripped_output" = "$expected_output" ]
}
@test "useless input" {
    run ./dsh <<EOF
hi
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    
    expected_output="dsh2>cmdloopreturned-6dsh2>dsh2>cmdloopreturned0"

    [ "$stripped_output" = "$expected_output" ]
    
}
@test "making and removing file" {
    run ./dsh <<EOF
touch file
ls
rm file
ls
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="batsdragon.cdshdsh_cli.cdshlib.cdshlib.hfilefork-exec-1.pngfork-exec-2.pngmakefilequestions.mdreadme.mdshell_roadmap.mdbatsdragon.cdshdsh_cli.cdshlib.cdshlib.hfork-exec-1.pngfork-exec-2.pngmakefilequestions.mdreadme.mdshell_roadmap.mddsh2>dsh2>dsh2>dsh2>dsh2>cmdloopreturned0"

    [ "$stripped_output" = "$expected_output" ]
    
}


