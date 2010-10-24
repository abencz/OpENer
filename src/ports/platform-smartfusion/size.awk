/\.text/{printf $2 "\t" $3 "\n"}
/\.glue_7/{printf $2 "\t" $3 "\n"}
/\.data/{printf $2 "\t" $3 "\n"}
/\.bss/{printf $2 "\t" $3 "\n"}

