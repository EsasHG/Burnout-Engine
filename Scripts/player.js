

var startPos;
function beginPlay()
{
    engine.setPlayerID(ID);
    startPos = engine.getPosition(ID);
    engine.playSound(ID);
}
function endPlay()
{
    engine.setPosition(startPos, ID);
}

function tick()
{

}

function onEntityCollision(otherEntity)
{
    if(otherEntity == engine.trophyID)
    {
        engine.debug("[Player::onEntityCollision]: Collided with trophy! " + otherEntity);
    }
    else
    {
        engine.setPosition(startPos, ID);
    }
}