
var speed =0.1;
var startPos;
function beginPlay()
{
    startPos = engine.getPosition(ID);
}

function endPlay()
{
    engine.setPosition(startPos, ID);
}

function tick()
{
    var playerPos = engine.getPosition(engine.getPlayerID());
    var pos = engine.getPosition(ID);
    var dir =  [
        (playerPos[0]- pos[0])/100,
        (playerPos[1]- pos[1])/100,
        (playerPos[2]- pos[2])/100
    ]

    var size = Math.sqrt(dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2]);

    dir = [ dir[0]/size*speed, dir[1]/size*speed, dir[2]/size*speed];
    engine.addInput(ID, dir);
}

function onEntityCollision(otherEntity)
{
    engine.setPosition(startPos, ID);
    engine.playSound(ID);
    //engine.debug("[AI::onEntityCollision]: Collided!")
}