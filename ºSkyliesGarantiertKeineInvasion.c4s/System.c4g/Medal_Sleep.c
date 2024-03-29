/*-- Neues Script --*/

#strict
#appendto MBOT

// EffectVar 0: Effect size, EffectVar 1: Culprit

public func Activate(object pCaster, object pRealcaster)
{
  // Effekte!
  Sound("Magic*", 0, this(), 70);
  var iRange=100;
  var iDuration = 37;
  var iAng, iX, iY, iRng;
  // Kombo: Stein verl�ngert Dauer
  var pComboObj;
  if (pComboObj = FindContents(ROCK, pCaster))
  {
    iDuration *= 2;
    RemoveObject(pComboObj);
  }
  // Kombo: Fisch und Kohle vergr��ert Radius
  pComboObj = FindContents(FISH, pCaster);
  if (!pComboObj) pComboObj = FindContents(DFSH, pCaster);
  if (!pComboObj) pComboObj = FindContents(COAL, pCaster);
  if (pComboObj)
    {
    if (GetAlive(pComboObj))
      iRange *= 3;
    else
      iRange *= 2;
    RemoveObject(pComboObj);
    }
  for(var i=iRange/2; i>0; --i)
  {
    iAng=Random(360);
    iRng=Random(iRange/2);
    iX=Sin(iAng, iRng);
    iY=Cos(iAng, iRng);
    CreateParticle("Feather", iX, iY, RandomX(-5,5), -3, 50+Random(25));
  }  
  var pClonk,pVictim;
  if(!(pClonk=pRealcaster)) pClonk=pCaster;
  // alle Clonks im n�heren Umkreis pr�fen
  while(pVictim=FindObject(0,-iRange/2,-iRange/2,iRange,iRange,OCF_CrewMember(),0,0,NoContainer(),pVictim))
    // die Opfer sind feindliche Clonks oder alle Clonks au�er dem Zauberer selbst wenn NoTeamMagic aktiviert ist
    if((Hostile(GetOwner(pVictim),GetOwner(pClonk)) || ObjectCount(NTMG)) && pClonk!=pVictim)
      // wirkt l�nger bei heruntergek�mpften Clonks
      AddEffect("SleepNSpell",pVictim,123,25,0,GetID(),iDuration*((115-GetEnergy(pVictim)*100000/GetPhysical("Energy",0, pVictim))/5), GetOwner(pCaster));
  return(1);
}

protected func FxSleepNSpellStart(object pTarget, int iEffectNumber, int iTemp, int iTime, int iCulprit)
{
  // tempor�re aufrufe
  if(iTemp) return();
  // Scheintoter Zustand
  ObjectSetAction(pTarget,"Dead");
  // mit schnarchen anfangen
  Sound("Snore",0,pTarget,0,0,+1);
  // Zeit bis zum Aufwachen speichern
  EffectVar(0,pTarget,iEffectNumber)=iTime;
  EffectVar(1,pTarget,iEffectNumber)=iCulprit;
  // Fertig
  return(1);
}

protected func FxSleepNSpellEffect(szNewEffect, pTarget, iEffectNumber)
  {
  // Wenn schon ein Schlafzauber wirkt, darf kein neuer dazukommen
  if (szNewEffect eq "SleepNSpell") return(-1);
  // Sonst keine Behandlung
  return();
  }

protected func FxSleepNSpellStop(object pTarget, int iEffectNumber, int iTime) 
{
  var pMedal = FindObject(SH_M);
  if(pMedal)
    pMedal->~AddSleepTime(GetOwner(pTarget), GetEffect(0, pTarget, iEffectNumber,6));
  // clonk hat den Schlaf nicht �berstanden ;)
  if(!GetAlive(pTarget)) return(1);
  // aufstehen
  ObjectSetAction(pTarget,"FlatUp",0,0,1);
  // mit schnarchen aufh�ren
  Sound("Snore",0,pTarget,0,0,-1);
  // stehenbleiben
  SetComDir(COMD_Stop(),pTarget);
  // Fertig
  return(1);
}

protected func FxSleepNSpellTimer(object pTarget, int iEffectNumber, int iTime) 
{
  var iMaxTime=EffectVar(0,pTarget,iEffectNumber);
  // nach einer bestimmten Zeit aufh�ren
  if(iTime>=iMaxTime) return(-1);
  // Alle SleepNSpellEffekte durchgehen und �berpr�fen, ob sie die gleiche EffectVar(1) haben. Wenn das bei mindestens 4 davon (diesen eingeschlossen) der Fall ist, Medaille vergeben!
  var sleepers = 0;
  for(var glonk in FindObjects(Find_OCF(OCF_CrewMember())))
  {
    for(var i = 1; i <= GetEffectCount("SleepNSpell",glonk); i++)
      if(EffectVar(1,glonk,GetEffect("SleepNSpell",glonk,i,0,0)) == EffectVar(1,pTarget,iEffectNumber))
	      sleepers++;

    //Message("%d",pTarget,sleepers);
    if(sleepers >= 4) 
    {
      var pMedal = FindObject(SM_M);
      if(pMedal)
        Reward(pMedal,EffectVar(1,pTarget,iEffectNumber));
    }
  }

  // Zzz-Partikel casten
  CreateParticle("Zzz",GetX(pTarget),GetY(pTarget),2+(iTime%50)*3/25+RandomX(-1,+1),-5,60,
                 RGBa(255,255,255,(iTime-iMaxTime/2)**2 * 180*4/iMaxTime**2));
}
