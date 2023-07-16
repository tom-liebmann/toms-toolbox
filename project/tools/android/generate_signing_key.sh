#!/bin/sh

STORE_NAME=$1

echo -n "Store Password: "
read -s STORE_PASSWORD
echo

echo -n "Key Password: "
read -s KEY_PASSWORD
echo

keytool -genkey -v \
        -dname "cn=Tom Liebmann, ou=, o=JuicyTrash, c=DE" \
        -keystore ${STORE_NAME}.jks \
        -keyalg RSA \
        -storetype pkcs12 \
        -keysize 2048 \
        -validity 10000 \
        -alias my-alias \
        -storepass ${STORE_PASSWORD} \
        -keypass ${KEY_PASSWORD}
