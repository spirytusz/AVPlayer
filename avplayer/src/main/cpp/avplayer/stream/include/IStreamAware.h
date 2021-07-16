
#ifndef AVPLAYER_ISTREAMAWARE_H
#define AVPLAYER_ISTREAMAWARE_H

class IStreamAware {
public:
    /**
     * 下游压力大，调用这个方法
     */
    virtual void OnHeavy() = 0;

    /**
     * 下游喘过气来了，调用这个方法
     */
    virtual void OnLight() = 0;
};

#endif //AVPLAYER_ISTREAMAWARE_H
