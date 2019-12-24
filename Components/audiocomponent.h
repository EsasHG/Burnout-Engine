#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include "basecomponent.h"
#include "Sound/soundsource.h"
/** Component needed for an entity to play sound.
 */
class AudioComponent : public BaseComponent
{
public:
    AudioComponent(unsigned inID) : BaseComponent(inID){}
    ~AudioComponent() override {delete mSoundSource;}
    virtual QJsonObject toJson() const override;
    virtual void fromJson(QJsonObject obj) override;
    float mGain = 1.0f;
    bool mShouldLoop = false;
    QString filename;
    SoundSource* mSoundSource = nullptr;


};

#endif // AUDIOCOMPONENT_H
