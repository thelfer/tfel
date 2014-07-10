#!/bin/bash

# This is fixbb version 0.31.  Copyright 2000 by Jeff Spirko.
# Redistributable under the GNU General Public License.
# See http://topquark.dhs.org/~spirko/fixbb/

# Changes by Petr Mikulik:
#   -  8.  9. 2000: working file in the $TMP directory; remove backup file
#   - 18. 10. 2000: AWKPARSEBB to support multiple-page (non-EPS) documents
#   - 26. 10. 2000: error if bounding box not determined (GS < 5.50 or empty
#     pages); page status info added

# Options for ghostscript to print the BoundingBox without other junk.
GSOPTS="-dQUIET -dBATCH -dNOPAUSE -sDEVICE=bbox"
export GSOPTS

# Original awk program, checks each line of the file
#AWKPROG='{ if ($1 == "%%BoundingBox:") 
#  print $1, ENVIRON["bbox"]; 
#  else print;}'

# Slightly faster version, doesn't bother looking after finding a BoundingBox.
# What happens if there is more than one BoundingBox?  Can this happen?
AWKPROG='
	(found) { print; next }
	$1 == "%%BoundingBox:" { print $1, ENVIRON["bbox"]; found=1; next }
	{ print }
	'

# Finds min and max of the 4 numbers after the "%%BoundingBox:" label
AWKPARSEBB='
	BEGIN { x1=99999; y1=99999; x2=0; y2=0; page=0 }
	$1!= "%%BoundingBox:" { next }
	$2<0 || $3<0 { next }
	x1>$2 { x1=$2 }
	y1>$3 { y1=$3 }
	x2<$4 { x2=$4 }
	y2<$5 { y2=$5 }
	{ page++; printf "Page " page ": " $0 "  max = " x1 " " y1 " " x2 " " y2  "  \t\r" >"/dev/stderr" }
	END { printf "\t\t\t\t\t\t\t\r" >"/dev/stderr"
	      print x1 " " y1 " " x2 " " y2 }
	'

export bbox

# Each command-line argument is assumed to be a file to process
for FILE in $* ; do

#  BAK=${FILE}~
#  if [ "$TMP" = "" ]; then TMP="."; fi; BAK=$TMP\fixbb__.ps
  BAK='fixbb$$.ps'

  # Get the correct BoundingBox from ghostscript.
  # Old: bbox=`gs ${GSOPTS} ${FILE} 2>&1 | gsort | head -1 | cut -d\  -f 2-`
  bbox=`/usr/bin/gs ${GSOPTS} ${FILE} 2>&1 | awk "$AWKPARSEBB"`

  case $bbox in
    "99999 99999 0 0")
	echo "Bounding box was NOT determined (empty pages or Ghostscript >= 5.50 required)"
	exit ;;
  esac

  echo "Bounding box for \""$FILE"\" is: "$bbox

  # Save the old version of the file
  mv $FILE $BAK

  # Create the new version, replacing the old BoundingBox
  awk "$AWKPROG" $BAK > $FILE

  case `uname` in 
    "OS/2") dos2unix $FILE ;;
  esac

  # delete the backup file
  rm -f $BAK

done
