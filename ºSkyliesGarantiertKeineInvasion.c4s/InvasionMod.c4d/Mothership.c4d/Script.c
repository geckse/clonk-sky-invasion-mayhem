/* Luftschutzkraftfeld */

#strict 2

local wxDir;
local wyDir;

/* Initialisierung */

protected func Initialize()
{
  wxDir = 0;
  wyDir = 0;
  Sound("Energize");
  SetOwner(NO_OWNER);
  SetAction("Field");
}


protected func Check(){
  if(GetXDir() > wxDir) SetXDir(GetXDir()/2);
  if(GetYDir() > wyDir) SetYDir(GetYDir()/2);
  if(GetXDir() < wxDir) SetXDir(GetXDir()/2);
  if(GetYDir() < wyDir) SetYDir(GetYDir()/2);
}
