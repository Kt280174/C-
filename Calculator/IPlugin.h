// IPlugin.h
#ifndef IPLUGIN_H
#define IPLUGIN_H

class IPlugin {
public:
    virtual ~IPlugin() {}
    virtual const char* get_name() const = 0;
    virtual int get_num_params() const = 0;  // Số lượng tham số cần thiết
    virtual double execute(double param1, double param2 = 0) = 0; // Hàm execute hỗ trợ 2 tham số
};

#endif // IPLUGIN_H
