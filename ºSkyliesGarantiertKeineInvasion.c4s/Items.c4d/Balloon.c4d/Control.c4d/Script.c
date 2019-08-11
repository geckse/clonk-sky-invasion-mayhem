/*-- Removal Tool --*/

#strict
local toRem;

func Initialize()
{
  return(1);
}

func SetToRem(pToRem)
{
  toRem = pToRem;
}

func Activate(pCaller)
{
  SetPlrView(GetOwner(pCaller), toRem);
  CreateMenu(GetID(), pCaller, , C4MN_Extra_None, "Zerst�ren?", , C4MN_Style_Normal, false);
  AddMenuItem("Zerst�ren!", "Boom", BLND, pCaller, 0, 0, "Soll der Ballon zerst�rt werden und das verbundene Objekt abgeworfen?", 0, this(), pCaller);
}

func Boom()
{
  SetPlrView(GetOwner(Contained()),LocalN("MyToy",toRem));
  toRem->DoHit();
  this()->RemoveObject();
  return(1);
}

func Timer()
{
  if(!toRem) RemoveObject();
  return(1);
}