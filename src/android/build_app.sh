set -e

ANDROID_SDK="/home/tom/libraries/android_sdk"
ANDROID_JAR="${ANDROID_SDK}/platforms/android-30/android.jar"
ANDROID_AAPT2="${ANDROID_SDK}/build-tools/30.0.3/aapt2"
ANDROID_DX="${ANDROID_SDK}/build-tools/30.0.3/dx"
ANDROID_ZIPALIGN="${ANDROID_SDK}/build-tools/30.0.3/zipalign"

JAVA_COMPILER="javac"
JAVA_SIGNER="jarsigner"

SCRIPT_DIR=$(dirname "$0")
OUT_DIR="${SCRIPT_DIR}/output"

rm -rf "${OUT_DIR}"
mkdir -p "${OUT_DIR}"

mkdir -p "${OUT_DIR}/java"

${JAVA_COMPILER} \
    -classpath "${ANDROID_JAR}" \
    -source 1.8 \
    -target 1.8 \
    -d "${OUT_DIR}/java" \
    "${SCRIPT_DIR}/src/MainActivity.java"

mkdir -p "${OUT_DIR}/dex"

${ANDROID_DX} \
    --dex \
    --output="${OUT_DIR}/classes.dex" \
    "${OUT_DIR}/java"

mkdir -p "${OUT_DIR}/res"

${ANDROID_AAPT2} \
    compile \
    "${SCRIPT_DIR}/res/layout/main.xml"


${ANDROID_AAPT2} \
    link \
    -I "${ANDROID_JAR}" \
    --manifest "${SCRIPT_DIR}/AndroidManifest.xml" \
    -v \
    -o "${OUT_DIR}/app.apk"

zip -uj "${OUT_DIR}/app.apk" \
    "${OUT_DIR}/classes.dex"

${ANDROID_ZIPALIGN} -f 4 "${OUT_DIR}/app.apk" "${OUT_DIR}/app_unsigned.apk"

${JAVA_SIGNER} \
    -keystore "${HOME}/.android/debug.keystore" \
    -storepass "android" \
    -keypass "android" \
    -signedjar "${OUT_DIR}/app_signed.apk" \
    "${OUT_DIR}/app_unsigned.apk" \
    "androiddebugkey"
