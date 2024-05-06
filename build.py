#!/usr/bin/env python3

import subprocess
import sys
import os

image_name = "patelkharsh2/duo-sdk"

def build(build_commands):
    current_directory = os.getcwd()
    docker_command = ["docker", "run", "-v", f"{current_directory}:/app", image_name] + build_commands
    subprocess.run(docker_command)

def run_docker_command(command):
    command_str = ' '.join(command)
    build_command = ""

    if "cmake" in command_str and "--build" in command_str:
        build_command = ['bash', '-c', f'cd /app/build && {command_str}']
    elif "cmake" in command_str and ".." in command_str:
        build_command = ['bash', '-c', f'mkdir -p /app/build && cd /app/build && {command_str}']
    else:
        build_command = command_str.split()

    build(build_command)

if __name__ == "__main__":
    run_docker_command(sys.argv[1:])