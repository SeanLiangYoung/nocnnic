# X53 #
_2010-02-09, 12:26:35_

> ## 修改Bug ##
    * [Issue #8](https://code.google.com/p/nocnnic/issues/detail?id=#8)
    * [Issue #9](https://code.google.com/p/nocnnic/issues/detail?id=#9)

# X50.1 #
_2010-02-07, 20:26:18_

> ## 修改Bug ##
    * [Issue #5](https://code.google.com/p/nocnnic/issues/detail?id=#5)

# X50 #
_2010-02-07, 14:47:57_

> ## 功能介绍 ##
> "CNNIC CA根证书移除工具"，将会把Firefox 3.6 Windows版中的**“CNNIC ROOT”**和**“Entrust.net Secure Server Certification Authority”**两个内嵌CA根证书修改为不可信任状态，其结果是所有使用这两个根证书签名的WEB网站证书、电子邮件证书、软件签名证书都将会被Firefox 3.6报告期证书持有者不可信任。

> 已测试：Windows XP SP3, Windows 2003R2, Windows 7

> ## 已知问题 ##
    * 如 [Issue #3](https://code.google.com/p/nocnnic/issues/detail?id=#3) 报告，如果Firefox 3.6中有用户导入的私人或者其他第三方证书，并且使用了本程序，那么用户将丢失所有私人或者第三方证书。
    * 如 [Issue #1](https://code.google.com/p/nocnnic/issues/detail?id=#1) 报告，本程序并不支持多个Firefox用户账户(Profile)。
    * 如 [Issue #2](https://code.google.com/p/nocnnic/issues/detail?id=#2) 报告，本程序不能检测一台计算机或者一个用户的Firefox账户(Profile)是否已经删除了CNNIC ROOT和Entrust.net Secure Server Certification Authority的CA根证书，所以用户有可能重复操作。

> ## 未来计划 ##
> > 解决如下问题

  * [Issue #1](https://code.google.com/p/nocnnic/issues/detail?id=#1)
  * [Issue #2](https://code.google.com/p/nocnnic/issues/detail?id=#2)
  * [Issue #3](https://code.google.com/p/nocnnic/issues/detail?id=#3)