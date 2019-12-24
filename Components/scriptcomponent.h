#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include "basecomponent.h"
#include <QJSEngine>
#include <QString>

/** Holds the filename of a script, and has a dedicated QJSEngine* for reading it.
 */
class ScriptComponent : public BaseComponent
{
public:
    ScriptComponent(unsigned inID) : BaseComponent(inID){}
    ~ScriptComponent() override{}
    virtual void fromJson(QJsonObject obj) override;
    virtual QJsonObject toJson() const override;

    QString mFileName = "";
    QJSEngine* mEngine = nullptr;
};

#endif // SCRIPTCOMPONENT_H
