# Webserver_with_CPP

<p> 简介：<br>
这是一个项目经验为0的菜鸟从0开始爬的山。
</p>

## 完成进度

### 2025/9/19

<p> 完成最基础的socket服务器搭建，服务器返回固定的HTTP消息。</p>

### 2025/9/21
<p> 用epoll,使服务器支持基本的多客户端连接。</p>

## 2025/9/22
<p> 使用OOP的方式重新构建服务端。 </p>

## 2025/9/23
<p> 在服务端添加 Channel 使 epoll 和 fd 对应,解耦 Epoll 中关于fd事件和回调处理的部分,并构建 EventLoop 解耦事件循环,
至此一个Epoll负责底层注册、Channel负责保存fd事件和绑定回调函数、EventLoop负责事件循环的简易的 Reactor 模型构建完毕。 </p>


