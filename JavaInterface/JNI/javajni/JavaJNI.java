package javajni;

public class JavaJNI
{

private native void nativePrint();

static {
    System.load("/mnt/c/Users/agniy/4-sem/JavaInterface/JNI/JNIdll/libJNIdll.so");
}

public static void main(String[] args)
{
    JavaJNI j = new JavaJNI();
    j.nativePrint();
}


}
