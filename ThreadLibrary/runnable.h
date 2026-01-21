class runnable {
public:
    virtual void run() = 0;
    virtual ~runnable() {};
    virtual bool stopRequested() const = 0;
};