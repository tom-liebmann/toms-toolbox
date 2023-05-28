#!/bin/sh

keytool -genkey -v \
        -dname "cn=Tom Liebmann, ou=, o=JuicyTrash, c=DE" \
        -keystore my-release-key.jks \
        -keyalg RSA \
        -storetype pkcs12 \
        -keysize 2048 \
        -validity 10000 \
        -alias my-alias \
        -storepass 123456 \
        -keypass 123456
