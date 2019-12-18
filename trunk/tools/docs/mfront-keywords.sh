#echo "# List of mfront keywords"      >> out.txt
for d in $(mfront --list-dsl |awk 'NR>1 {print $2}'|grep -v Parser);
do
    mfront --help-keywords=$d &> $d-keywords.md
done
