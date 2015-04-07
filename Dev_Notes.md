# 编译Windows版nocnnic #

> 编译nocnnic需求的工具是[MinGW](http://mingw.org/)。

> 将nocnnic的[代码](http://code.google.com/p/nocnnic/source/list)用hg clone之后，设置环境变量 “ **MINGW\_PATH**” 将其指向MinGW安装路径，然后在代码根目录之win下执行mingw32-make即可编译完成。编译结果的二进制文件在bin目录下。

# Windows版nocnnic功能实现机制 #

> 到X53版本，Windows版nocnnic完成的功能仅仅是找到Firefox的用户目录，将一个去掉了“CNNIC ROOT”CA根证书和“Entrust.net Secure Server Certification Authority”CA根证书的默认证书库（[cert8.db](http://code.google.com/p/nocnnic/source/browse/win/res/cert8.db.h)和[key3.db](http://code.google.com/p/nocnnic/source/browse/win/res/key3.db.h)）覆盖用户的当前证书库。