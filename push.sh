#!/bin/bash
git rm -r --cached . > /dev/null
git add .
git commit -m "Regular update."
git push origin

