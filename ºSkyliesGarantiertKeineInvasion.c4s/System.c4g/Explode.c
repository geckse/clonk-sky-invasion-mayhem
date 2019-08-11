/* BlastObjects �nderung */
#strict
  
// Objekte besch�digen und wegschleudern
global func BlastObjects(int x, int y, int level, object container, int cause_plr_plus_one, object layer)
  {
  var obj;
  
  // Im Container?
  if (container)
    {
    if (GetObjectLayer(container) == layer)
      {
      BlastObject(level, container, cause_plr_plus_one);
      for (obj in FindObjects(Find_Container(container), Find_Layer(layer)))
        BlastObject(level, obj, cause_plr_plus_one);
      }
    }
  else {
    // Objekte am Explosionspunkt besch�digen
    for (var obj in FindObjects(Find_AtRect(x-5, y-5, 10,10), Find_NoContainer(), Find_Layer(layer)))
      BlastObject(level, obj, cause_plr_plus_one);
    
    var shockwave_objs = FindObjects(Find_Distance(level, x,y), Find_NoContainer(), Find_Layer(layer),
      Find_Category(C4D_Living|C4D_Vehicle), Find_Func("BlastObjectsShockwaveCheck",x,y));
    var cnt = GetLength(shockwave_objs);
    if (cnt)
      {
      // Die Schleuderenergie teilt sich bei vielen Objekten auf
      //Log("Shockwave objs %v (%d)", shockwave_objs, cnt);
      var shock_speed = Sqrt(2 * level * level / BoundBy(cnt, 2, 12));
      for (var obj in shockwave_objs) if (obj) // obj noch pr�fen, weil OnShockwaveHit Objekte aus dem Array l�schen k�nnte
        {
        // Objekt hat benutzerdefinierte Reaktion auf die Schockwelle?
        if (obj->~OnShockwaveHit(level, x,y)) continue;
        // Lebewesen leiden besonders
        var cat = GetCategory(obj);
        if (cat & C4D_Living)
          {
          DoEnergy(level/-2, obj, false, FX_Call_EngBlast, cause_plr_plus_one);
          DoDamage(level/2, obj, FX_Call_DmgBlast, cause_plr_plus_one);
          }
        // Killverfolgung bei Projektilen
        if (cat & C4D_Object) SetController(cause_plr_plus_one, obj);
        // Schockwelle
        var mass_fact = 20, mass_mul = 100; if (cat & C4D_Living) { mass_fact = 8; mass_mul = 80; }
        mass_fact = BoundBy(GetMass(obj)*mass_mul/1000, 4, mass_fact);
        var dx = 100*(GetX(obj)-x)+Random(51)-25;
        var dy = 100*(GetY(obj)-y)+Random(51)-25;
        var vx, vy;
        if (dx)
          {
          vx = Abs(dx)/dx * (100*level-Abs(dx)) * shock_speed / level / mass_fact;
          }
        vy = (Abs(dy) - 100*level) * shock_speed / level / mass_fact;
        if (cat & C4D_Object)
          {
          // Objekte nicht zu schnell werden lassen
          var ovx = GetXDir(obj, 100), ovy = GetYDir(obj, 100);
          if (ovx*vx > 0) vx = (Sqrt(vx*vx + ovx*ovx) - Abs(vx)) * Abs(vx)/vx;
          if (ovy*vy > 0) vy = (Sqrt(vy*vy + ovy*ovy) - Abs(vy)) * Abs(vy)/vy;
          }
        //Log("%v v(%v %v)   d(%v %v)  m=%v  l=%v  s=%v", obj, vx,vy, dx,dy, mass_fact, level, shock_speed);
        Fling(obj, vx,vy, 100, true);
        }
      }
    }
  // Fertig
  return true;
}
