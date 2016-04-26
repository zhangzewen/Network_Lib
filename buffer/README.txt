buffer装饰器实现想法：
1. baseBuffer不提供对文件描述符号的存储
2. baseBuffer作为最基础的组件，其实现过程是直接参考libevent1.4.x版本的buffer的实现，做了细微的修改
3. 目前没有给出对buffer的直接操作，准备提供萃取功能，能适配stl模版，提供操作数据的接口
4. 目前只支持单线程操作，今后也不打算实现多线程，
5. 后续会实现fileBuffer, netBuffer, doubleBuffer, ringBuffer;
6. 用到internet里面去
8. 这周一定要完成
