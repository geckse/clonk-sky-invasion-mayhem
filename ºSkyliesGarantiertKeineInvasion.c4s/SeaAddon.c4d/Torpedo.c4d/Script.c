/*-- Torpedo --*/

#strict

local Launched;   // Ist der Torpedo scharf?
local LaunchTime; // Minimale Laufzeit vor einem Treffer

local LaunchedThrow;
local iWR;
local iR;
/* Initialisierung */

protected func Initialize()
{
  SetAction("Waiting");
  SetComDir(COMD_Stop());
}
public func DiAngle(){
  if(GetXDir()){  SetR(Angle(GetX(),GetY(),GetX()+GetXDir(),GetY()+GetYDir())+90,this());      }
}
public func Launch(object by, int r)
{
  // Torpedo scharf machen
  Launched = 1;
  LaunchTime = 30;
  // Waagerecht: Nach abwerfendem Objekt ausrichten
  if (by)
  {
    SetAction("Swim");
    SetDir(GetDir(by));
    SetXDir(GetXDir(by));
    SetYDir(0);
    if (GetDir()==DIR_Right()) SetComDir(COMD_Right());
    if (GetDir()==DIR_Left()) SetComDir(COMD_Left());
  }
  // Gedreht
  else
  {
    SetAction ("SwimR");
    SetR(r + 90);
    SetXDir(0);
    SetYDir(0);
    SetComDir(COMD_None());
  }
  return(1);
}

/* Einsammeln */

public func Entrance(object container)
{
  // Schwimmaktion beenden
  SetAction("Waiting");
}

/* Aktivit�t */

private func Swimming()
{

  if(!Launched){ if(!LaunchedThrow){  LaunchedThrow = true; SetRDir(0); iR = GetR();}  }
  // Gedreht: wir sollten die SwimR-Aktion verwenden
  if (GetR()) return(SetAction("SwimR"));
  // Bewegung
  if (Launched)
  {
    // Effekte
    var iDir=(GetDir()*2-1);
    Bubble(-16*iDir,0);
    Bubble(-16*iDir,0);
    // Nach einer gewissen Zeit nur noch horizontal bewegen
    if (GetActTime() > LaunchTime) SetYDir(0);
    // Trefferkontrolle
    HitCheck();
  }
  // Bewegung
  if (LaunchedThrow)
  {
    // Effekte
    var iDir=(GetDir()*2-1);
    Bubble(-16*iDir,0);
    Bubble(-16*iDir,0);
    // Nach einer gewissen Zeit nur noch horizontal bewegen
    if (GetActTime() > LaunchTime) SetYDir(0);
    // Trefferkontrolle
    HitCheck();
  }

}

private func SwimmingR()
{
  if(!Launched){ if(!LaunchedThrow){  LaunchedThrow = true; SetRDir(0); iR = GetR();}  }

  // Bewegung
  if (Launched)
  {
    // Bewegung
    SetXDir(-Cos(GetR(), 245000) * 2, 0, 100000);
    SetYDir(-Sin(GetR(), 245000) * 2, 0, 100000);
    // Effekte
    Bubble(Cos(GetR(), 16), Sin(GetR(), 16));
    // Trefferkontrolle
    HitCheck();
  }
  // Bewegung nach Wurf
  if (LaunchedThrow)
  {
    if(GetXDir() > 0){ iWR = -183+RandomX(0,6); }
    if(GetXDir() < 0){ iWR = 3-RandomX(0,6); }
    if(iR < iWR){ iR+=5; }
    if(iR > iWR){ iR-=5; }
    SetR(iR);
    // Bewegung
    SetXDir(-Cos(GetR(), 245000) * 2, 0, 100000);
    SetYDir(-Sin(GetR(), 245000) * 2, 0, 100000);
    // Effekte
    Bubble(Cos(GetR(), 16), Sin(GetR(), 16));
    // Trefferkontrolle
    HitCheck();
  }

}

private func StartJump()
{
  // Zuf�llige Drehung
  SetRDir(Random(21) - 10);
}

private func HitCheck()
{
  var pItem = FindObject2(Find_Distance(12),Find_OCF(OCF_Collectible),Find_NoContainer(),Find_Exclude(this()));

  if(pItem) {
   if(Launched) SetRDir(RandomX(-5,5));
   if(LaunchedThrow){ SetRDir(RandomX(-5,5)); LaunchedThrow=0; Launched = 1; }
   SetXDir(RandomX(0,GetXDir()/2),pItem);
   SetYDir(RandomX(-2,-12),pItem);
   pItem->~Hit();
   }
  var pTarget;
  // Erst nach minimaler Laufzeit
  if (GetActTime() > LaunchTime)
    // M�gliche Zielobjekte suchen
    while (pTarget = FindObject(0, Cos(GetR(),-20),Sin(GetR(),-20), 0,0, 0, 0,0, NoContainer(), pTarget))
      // Es werden Gegenst�nde, Lebewesen, Fahrzeuge und Geb�ude getroffen
      if (pTarget->GetCategory() & (C4D_Object() | C4D_Living() | C4D_Vehicle() | C4D_Structure()))
        // Treffer
        Hit();
}

/* Schaden */

public func Damage()
{
  // Zerst�rung
  Hit();
}

/* Aufschlag */

public func Hit() { Explode(30); }

/* Produkteigenschaften */

public func IsChemicalProduct() { return(1); }

/* Status */

public func IsTorpedo() { return(1); }

/* Forschung */

public func GetResearchBase() { return(SUB1); }

public func IsDeepSeaResearch() { return(1); }