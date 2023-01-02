#ifndef ADMFS_SINGLETON_HH__
#define ADMFS_SINGLETON_HH__

template <class T>
class singleton_holder
{ /**< simple Meyers' style singleton */
public:
    typedef T obj_type;
    static T& instance() /**< @return instance of the object */
    {
	static T obj;
	return obj;
    }
private:
    //all this operations are unavailable to ordinal users
    singleton_holder();
    singleton_holder(const singleton_holder& s);
    singleton_holder& operator=(const singleton_holder& s);
    ~singleton_holder();
};

#endif

