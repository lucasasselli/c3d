#ifndef _DELTA_H_
#define _DELTA_H_

template<class T>
class Delta {

public:
    Delta();
    void sample(T);
    T get();

private:
    int init;
    T oldValue;
    T newValue;
};

template<class T>
Delta<T>::Delta()
{
    init = false;
}

template<class T>
void Delta<T>::sample(T x)
{
    oldValue = newValue;
    newValue = x;
    init = true;
}

template<class T>
T Delta<T>::get()
{
    if(init){
        return newValue - oldValue;
    }else{
        return 0;
    }
}

#endif