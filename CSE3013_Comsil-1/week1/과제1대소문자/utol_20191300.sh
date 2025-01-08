#!/bin/bash
echo "Working directory" #/sogang/under/cse20131251/shell 
#echo $pwd
read dirname #directory 이름을 입력을 저장
#echo "read ok"
#echo "$dirname"
flag=0

if [ -n "$dirname" ]  #dirname의 길이가 0이 아니면
then
    if [ -d "$dirname" ] #dirname인 디렉토리가 존재하면 
    then 
        cd $dirname
        if [ $? -ne 0 ]   
        then
            echo "Cannot access directory "$dirname""
            exit 0
        fi
    else  
        echo "No directory "$dirname""
        exit 0
    fi

fi
for dir in *
do
    #echo $dir
    #echo 'first'
    if [ -f "$dir" ]
    then
        newname=`echo $dir | tr "[a-z] [A-Z]" "[A-Z] [a-z]"` #변수 newname은 dir의 대소문자를 tr을 이용해서 바꾼 것
        #echo $newname
        #echo 'second'
        mv $dir $newname #$dir을 newname으로 바꾼다. 
    fi
done
