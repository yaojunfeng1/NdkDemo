# 设置Cmake最小版本
cmake_minimum_required(VERSION 3.4.1)

# 编译library
add_library( # 设置library名称
             native-test1

            # 设置library模式
            # SHARED模式会编译so文件，STATIC模式不会编译
             SHARED

            # 设置原生代码路径
             native-test.cpp )
add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # 设置原生代码路径
             native-lib.cpp md5.cpp)


# 用来定位的library
find_library( # 设置library名称
              log-lib

              # 将library路径存储为一个变量，可以在其他地方用这个变量引用NDK库
              # 在这里设置变量名称
              log )


target_link_libraries( # 关联的library
                        native-test1
                        native-lib

                        # 关联native-lib和log-lib
                       ${log-lib} )