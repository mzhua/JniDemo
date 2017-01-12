#include <jni.h>
#include <string>

static jclass contextClass;
static jclass signatureClass;
static jclass packageNameClass;
static jclass packageInfoClass;

const char* AUTH_KEY = "0CC175B9C0F1B6A831C399E269772661";

/**
 * 发布的app 签名,只有和本签名一致的app 才会返回合法的 授权 key
 */
const char* RELEASE_SIGN ="3082030d308201f5a0030201020204163677d7300d06092a864886f70d01010b05003037310b30090603550406130255533110300e060355040a1307416e64726f6964311630140603550403130d416e64726f6964204465627567301e170d3134303131343035333835315a170d3434303130373035333835315a3037310b30090603550406130255533110300e060355040a1307416e64726f6964311630140603550403130d416e64726f696420446562756730820122300d06092a864886f70d01010105000382010f003082010a02820101008f02a948189bfa0134bdb17155061a4b62956c08881eac87b39f0a0acf1c808596100b09a16600f5567e5504f035f1deadc1dcc93fa93b930f78ac9a55ee961283458d66d3775e612e4eb5f5076bf06aeabb6e285400e89dd6e1327a52b4739a91d04d7288bc64c336b7776efe0b6341913d655943ffc9da4b8ba157a52f7790a815bf0cc693dd684209a07290b9c8d4220c8f56c00c25bee78500231213adba58b3c2c9e95951308dfab9edc4f9744fe6c99b3311b54971210db4c2e8a5d518def97535a69170827d6c14bd8a822a0d1b179a3633178db16376a9596df91c595b8cb3d3a916b0f10d029a0aad3b345a7ad54e85f2547686f612b62a010d7dd90203010001a321301f301d0603551d0e04160414f7c342dca7f916bb77312bb517105732c5e96328300d06092a864886f70d01010b0500038201010010b1bb9c29118a7eb15193ef15d40bafd4338526b200511f0e348e5c93eb9cbdf9330163cba183022572513dc7dac14cbd384c116bfc2c3d5efc9f545c3972ec8ce32908eb4e54a6940774ac569d682c188388380d60ed1904ecca4f4bf6bdcacd8be71557f1133c2fd5bcc2577fde19adef54cb91e02e635b47e6a6bfb1f46a5956259a10c61727e70cd8595fd632d4ed6c588dcb089967f164090aa89050911fd70eb1f7ccacd4d7b75c0b5ecd2bb84709b16176b9ac21f8bb25a970f65105af7939057cbf616abf5e9617f1ef87e39d16e458b278cbdc4f9ccb5d8e0c69719a5fae55bdd786b7b59c4a37a205972de1b0f3d4e8877267bad8f28ab09800f2";

extern "C"
JNIEXPORT jstring JNICALL
Java_im_hua_jnidemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject, contextObject) {

    jmethodID getPackageManagerId = (env)->GetMethodID(contextClass, "getPackageManager","()Landroid/content/pm/PackageManager;");
    jmethodID getPackageNameId = (env)->GetMethodID(contextClass, "getPackageName","()Ljava/lang/String;");
    jmethodID signToStringId = (env)->GetMethodID(signatureClass, "toCharsString","()Ljava/lang/String;");
    jmethodID getPackageInfoId = (env)->GetMethodID(packageNameClass, "getPackageInfo","(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jobject packageManagerObject =  (env)->CallObjectMethod(contextObject, getPackageManagerId);
    jstring packNameString =  (jstring)(env)->CallObjectMethod(contextObject, getPackageNameId);
    jobject packageInfoObject = (env)->CallObjectMethod(packageManagerObject, getPackageInfoId,packNameString, 64);
    jfieldID signaturefieldID =(env)->GetFieldID(packageInfoClass,"signatures", "[Landroid/content/pm/Signature;");
    jobjectArray signatureArray = (jobjectArray)(env)->GetObjectField(packageInfoObject, signaturefieldID);
    jobject signatureObject =  (env)->GetObjectArrayElement(signatureArray,0);

    const char* signStrng =  (env)->GetStringUTFChars((jstring)(env)->CallObjectMethod(signatureObject, signToStringId),0);
    if(strcmp(signStrng,RELEASE_SIGN)==0)//签名一致  返回合法的 api key，否则返回错误
    {
        return (env)->NewStringUTF(AUTH_KEY);
    }else
    {
        return (env)->NewStringUTF("error");
    }

}


JNIEXPORT jint JNICALL JNI_OnLoad (JavaVM* vm,void* reserved){

    JNIEnv* env = NULL;
    jint result=-1;
    if(vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
        return result;

    contextClass = (jclass)env->NewGlobalRef((env)->FindClass("android/content/Context"));
    signatureClass = (jclass)env->NewGlobalRef((env)->FindClass("android/content/pm/Signature"));
    packageNameClass = (jclass)env->NewGlobalRef((env)->FindClass("android/content/pm/PackageManager"));
    packageInfoClass = (jclass)env->NewGlobalRef((env)->FindClass("android/content/pm/PackageInfo"));

    return JNI_VERSION_1_4;
}