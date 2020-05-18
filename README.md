# Arcsoft Face

[![GitHub issues](https://img.shields.io/github/issues/ghtz08/arcsoft-face?style=flat-square)](https://github.com/ghtz08/arcsoft-face/issues)
[![GitHub forks](https://img.shields.io/github/forks/ghtz08/arcsoft-face?style=flat-square)](https://github.com/ghtz08/arcsoft-face/network)
[![GitHub stars](https://img.shields.io/github/stars/ghtz08/arcsoft-face)](https://github.com/ghtz08/arcsoft-face/stargazers)

对 ArcSoft 公司提供的 ArcFace 使用 C++ 进行简单的封装，并提供线程安全的接口

## 待改进

- [ ] 人脸角度的封装。
- [ ] 单人脸可以直接转为 SDK 的多人脸。
- [ ] 多人脸直接封装为一个类，而不是用 vector 包装，内部依然和 ASF_MultiFaceInfo 采用相同的实现以免可以快速的转换到 ASF_MultiFaceInfo，通过提供遍历单个人脸的所有信息以及每个人脸的每一个信息。
