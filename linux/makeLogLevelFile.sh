#!/bin/bash

if [[ ! -f "$1" ]]; then
    echo 0 > "$1"
fi