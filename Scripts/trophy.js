function beginPlay()
{
    engine.trophyID = ID;
    engine.debug("[Trophy::beginPlay]:!");
    let dir;
    dir= [1,0,0]; 
    engine.addInput(ID, dir)
}
function endPlay()
{
    //engine.setPosition([0,0,0], ID);
    //engine.setVisibility(true, ID);
}

function tick()
{
    //engine.debug("[Trophy::tick]:!");

}

function onEntityCollision(otherEntity)
{
    if(otherEntity == engine.getPlayerID())
    {

        engine.playSound(ID);
        let pos;
        pos = [ engine.randInRange(-50,50), 30, engine.randInRange(-50,50)];
        engine.setPosition(pos, ID);
        let dir;
        dir = [1,0,0]; 
        engine.addInput(ID, dir)
    }
}