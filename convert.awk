#!/usr/bin/awk -f 
/---/ {$0 = ""}
{gsub("\\|", " ", $0); gsub("  ", " 0 ", $0); print $0;}
