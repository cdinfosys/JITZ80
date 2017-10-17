@echo off
echo "GenerateClass <class name> <namespace> <guard condition>"
sed -e "s/${CLASSNAME}/%1/g" -e "s/${NAMESPACE}/%2/g" -e "s/${GUARD}/%3/g" ClassHPPTemplate.templ > "..\%2\%1.hpp"
sed -e "s/${CLASSNAME}/%1/g" -e "s/${NAMESPACE}/%2/g" -e "s/${GUARD}/%3/g" ClassCPPTemplate.templ > "..\%2\%1.cpp"
echo ">>>> ..\%2\%1.hpp"
echo ">>>> ..\%2\%1.cpp"
