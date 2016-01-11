#!/bin/bash
python camera.py | ssh ivy.millennium.berkeley.edu 'ssh root@192.168.1.1 "LD_LIBRARY_PATH=. ./new-fesvr ./pk hwacha-echo"' | python display.py
