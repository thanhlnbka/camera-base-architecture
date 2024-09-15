```sh
├── CMakeLists.txt
├── include
│   ├── capture
│   │   ├── api_capture_task.h
│   │   ├── capture_task.h
│   │   ├── rtsp_capture_task.h
│   │   └── zmq_capture_task.h
│   ├── handler
│   │   └── handler.h
│   ├── pose
│   │   └── pose_task.h
│   ├── track
│   │   └── track_task.h
│   └── utils
│       ├── dynamic_properties.hpp
│       └── limited_queue.hpp
├── libs
│   └── liblogger
│       ├── include
│       └── src
├── main.cpp
└── src
    ├── capture
    │   ├── capture_task.cpp
    │   └── rtsp_capture_task.cpp
    ├── pose
    │   └── pose_task.cpp
    └── track
        └── track_task.cpp

```