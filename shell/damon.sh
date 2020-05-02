#!/bin/sh

pwd=`dirname $0`
cd $pwd
dics=`ls`


# get the max number in a list
getMax(){
    local max=0
    for i in $1
    do
        echo $i
        if [ $i -gt $max ];then
            max=$i
        fi
    done
    return $max
}

runApp(){
    versions=`ls | grep $1 | awk -F_ '{print $2}' | sort`
    
    getMax $versions
    max=$?
    echo "max = $max"
    appToRun=`ls | grep \_$max\_`    
    md5File=`echo $appToRun | awk -F_ '{print $3}'`
    md5Check=`md5sum $appToRun | awk '{print $1}'`
    if [ "$md5File" != "$md5Check" ];then
        echo "file md5 check error"
        echo "file md5 = $md5File"
        echo "actuall md5 = $md5Check"
        return -1;
    fi
    chmod 777 $appToRun
    ./$appToRun
}



for dic in $dics
do
    if [ -d $dic ];then
        cd $dic
        runApp $dic
        cd -
    fi
done







