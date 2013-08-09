#!/bin/bash

URL="http://icfpc2013.cloudapp.net/guess?auth=02202IyLDrkqBSOKrBETKmIgSohseDuWPELwNvXivpsH1H"

ID=$1
PROGRAM=$2

curl -d "{ \"id\":\"$ID\", \"program\":\"$PROGRAM\" }" $URL;
