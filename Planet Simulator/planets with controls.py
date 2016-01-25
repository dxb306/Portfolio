# -*- coding: utf-8 -*-
import pygame, sys
from pygame.locals import *
import random, math, time

pygame.init()
fpsClock = pygame.time.Clock()
frames = 0



FPS =30 # frames per second setting
nplanets = 250
minrad = 1
maxrad = 10
maxspeed = 3
alphalevel = 255
screenw = 1800
screenh = 1000
border = maxrad
BACKGROUND = (0,0,0)
Title = 'Planets'
borderteleport = 1
absorbcollisions = 1
followplanet = 0
pause = 0
controls = 0
friction = 0.
gravity = 1
billcollisions = 0

settings = [FPS,nplanets,minrad,maxrad,maxspeed,alphalevel,screenw,screenh,border,BACKGROUND,
                   Title,borderteleport,absorbcollisions,followplanet,pause,controls,friction,gravity,billcollisions]

#set up control text
fontObj = pygame.font.Font('freesansbold.ttf',32) #set control text font and size
textSurfaceObj = fontObj.render('CONTROLS', True, (255,255,255)) #render the actual word
textRectObj = textSurfaceObj.get_rect()
textRectObj.right = screenw
textRectObj.top = 0 #set it at top right of screen


labelfontObj = pygame.font.Font(None,20) #set label font
namelist = ['FPS','Number of Planets','Radius of Planets','Initial Speed','Trail Length','Friction',
            'Absorbing Collisions','Border Bouncing','Gravity','Billiard Collisions'] #set labels
labelsurfs = []
labelrects = []
offset = 20
for name in namelist:
    labelsurf = labelfontObj.render(name,True,(255,255,255))
    labelsurfs.append(labelsurf)
    labelrect = labelsurf.get_rect()
    labelrect.centerx = textRectObj.centerx
    labelrect.bottom = textRectObj.bottom + offset
    offset += 40
    labelrects.append(labelrect)
offset = 20
controlRects = []



#set up window
DISPLAYSURF = pygame.display.set_mode((screenw,screenh),0,32)
pygame.display.set_caption(Title)

#set up transparent surface for trails
s = pygame.Surface((screenw,screenh))  # the size of your rect
s.set_alpha(settings[5])                # alpha level of fade
s.fill(BACKGROUND)           # this fills the entire surface


#creates array which contains all planet info for drawing

def planetcreate (n):
    global settings
    count = 0
    planets = []
    while (count <= n):
        info = [(random.randint(50,255),random.randint(50,255),random.randint(50,255)),
                random.randint(border,screenw-border),random.randint(border,screenh-border),
                random.randint(settings[2],settings[3]),
                0,0,
                random.randint(-settings[4],settings[4]),random.randint(-settings[4],settings[4])]
        planets.append(info)
        count += 1
    return planets
    

#takes all planet info and draws to screen with faded background

def planetdraw(planets):
    global settings
    if(settings[5] == 255):
            DISPLAYSURF.fill(BACKGROUND)
    else:
            s.set_alpha(settings[5]) 
            DISPLAYSURF.blit(s, (0,0))    # (0,0) are the top-left coordinate

    for planet in planets:
        pygame.draw.circle(DISPLAYSURF,planet[0],(planet[1],planet[2]),planet[3],0)


#Works out gravitational attraction between all planets

def gravitycalc(planets):
    global settings
    for planet in planets:
        for fplanet in planets:
            if planet != fplanet:
                DX = (planet[1]-fplanet[1])
                DY = (planet[2]-fplanet[2])
                DIST = math.sqrt(DX*DX + DY*DY)
                M = planet[3]**3
                Mf = fplanet[3]**3
                G = 6.67*(10**(-11))
                if DIST > (planet[3]+fplanet[3]) and settings[17]:
                    planet[4] -= (Mf*DX)/(DIST*DIST*DIST)
                    planet[5] -= (Mf*DY)/(DIST*DIST*DIST)
                    
                elif  (DIST <= (planet[3]+fplanet[3])):
                    if(settings[12]):
                        planetgone = planets.pop(planets.index(fplanet))
                        planet[3] = int(round(((planet[3]**3 + planetgone[3]**3)**(1.0/3.0))))
                        planet[0] = tuple(map(lambda x, y: ((planet[3]**3)*x + (planetgone[3]**3)*y)/((planet[3]**3)+(planetgone[3]**3)), planet[0], planetgone[0]))
                        planet[1] = (M*planet[1] + Mf*planetgone[1])/(M + Mf)
                        planet[2] = (M*planet[2] + Mf*planetgone[2])/(M + Mf)
                        planet[6] = (M*planet[6] + Mf*planetgone[6])/(M + Mf)
                        planet[7] = (M*planet[7] + Mf*planetgone[7])/(M + Mf)
                    if(settings[18]):
                        if(DX!=0):
                            planet[4] += math.sqrt(settings[3]**3)*(DX*math.sqrt(Mf))/(DX*DX*math.sqrt(M))
                        if(DY!=0):
                            planet[5] += math.sqrt(settings[3]**3)*(DY*math.sqrt(Mf))/(DY*DY*math.sqrt(M))
                    '''
                    if(settings[18]):
                        if(DX!=0) and (DY!=0):
                            cAng = math.atan(float(DX)/float(DY))
                        elif(DX==0):
                            cAng = 0
                        elif(DY==0):
                            cAng = math.pi/2
                        cVelplan = planet[6]*math.cos((math.pi/2)-cAng) + planet[7]*math.cos(cAng)
                        cVelfplan = (fplanet[7] + fplanet[6])*(math.cos(cAng))
                        cVelplan2 = (cVelplan*(M-Mf)+2*Mf*cVelfplan)/(M+Mf)
                        cVelfplan2 = (cVelfplan*(Mf-M)+2*M*cVelplan)/(M+Mf)
                        planet[6] = int(cVelplan2*math.sin(cAng))
                        planet[7] = int(cVelplan2*math.cos(cAng))
                        fplanet[6] = int(cVelfplan2*math.sin(cAng))
                        fplanet[7] = int(cVelfplan2*math.cos(cAng))
                    '''                    

                        
        if ((2*planet[6] + planet[4]) >= 0.5) or ((2*planet[6] + planet[4]) <= -0.5):
            planet[1] += 2*planet[6] + int(round(planet[4]))
            planet[6] = (planet[6] + int(round(planet[4])))
            planet[4] = 0
        if ((2*planet[7] + planet[5]) >= 0.5) or ((2*planet[7] + planet[5]) <= -0.5): 
            planet[2] += 2*planet[7] + int(round(planet[5]))
            planet[7] = (planet[7] + int(round(planet[5])))
            planet[5] = 0
        planet[6] = planet[6] - int((settings[16]/100)*planet[6])
        planet[7] = planet[7] - int((settings[16]/100)*planet[7])

    #changes all coordinates to follow one planet
    if(followplanet):
        if planets[0][1] != int(screenw/2):
            DX = planets[0][1]-int(screenw/2)
            for planet in planets:
                planet[1] -= DX
        if planets[0][2] != int(screenw/2):
            DY = planets[0][2]-int(screenh/2)
            for planet in planets:
                planet[2] -= DY

    #any planet outside of screen is redirected back in
    for planet in planets:
        if(settings[11]):
            if (planet[1] <= 0) or (planet[1]>=screenw):
                planet[6] = - planet[6]
                if(planet[1] < 0):
                    planet[1]+= 10
                    planet[6]+=1
                if(planet[1] > screenw):
                    planet[1]-=10
                    planet[6]-=1
            if (planet[2] <= 0) or (planet[2]>=screenh):
                planet[7] = -planet[7]
                if(planet[2] < 0):
                    planet[2]+= 10
                    planet[7]+=1
                if(planet[2] > screenh):
                    planet[2]-=10
                    planet[7]-=1
    
    return planetarray

def restart():
    global settings
    global planetarray
    DISPLAYSURF.fill(BACKGROUND)
    planetarray = planetcreate(settings[1])

def eventhandle(event):
    global settings
    global controlRects
    global planetarray
    if (event.type == QUIT):
        pygame.quit()
        sys.exit()
    if event.type == MOUSEBUTTONDOWN:
        mousepos = list(event.pos)
        mousex = mousepos[0]
        mousey = mousepos[1]
        if(textRectObj.collidepoint(event.pos)):
            controlsdraw()
        elif settings[15] == 1:
            controlnum = -1
            for rect in controlRects:
                controlnum+=1
                if(rect.collidepoint(event.pos)):
                    if namelist[controlnum] == 'FPS':
                        settings[0] = int(((mousepos[0]-rect.left)/float(rect.width))*60)
                    if namelist[controlnum] == 'Number of Planets':
                        settings[1] = int(((mousepos[0]-rect.left)/float(rect.width))*500)
                        restart()
                    if namelist[controlnum] == 'Radius of Planets':
                        settings[3] = int(((mousepos[0]-rect.left)/float(rect.width))*30)+2
                        restart()
                    if namelist[controlnum] == 'Initial Speed':
                        settings[4] = int(((mousepos[0]-rect.left)/float(rect.width))*10)
                        restart()
                    if namelist[controlnum] == 'Trail Length':
                        settings[5] = int(((rect.right-mousepos[0])/float(rect.width))*255)
                    if namelist[controlnum] == 'Friction':
                        settings[16] = ((mousepos[0]-rect.left)/float(rect.width))*100
                    if namelist[controlnum] == 'Absorbing Collisions':
                        if(settings[12] == 1):
                            settings[12]=0
                        elif(settings[12]==0):
                            settings[18] = 0
                            settings[12] = 1
                    if namelist[controlnum] == 'Border Bouncing':
                        if(settings[11]):
                            settings[11]=0
                        elif(settings[11]==0):
                            settings[11]=1
                    if namelist[controlnum] == 'Gravity':
                        if(settings[17]):
                            settings[17]=0
                        elif(settings[17]==0):
                            settings[17]=1
                    if namelist[controlnum] == 'Billiard Collisions':
                        if(settings[18] == 1):
                            settings[18]==0
                        elif(settings[18]==0):
                            settings[12] = 0
                            settings[18] = 1
                    controlnum = 0
                    settings[15] = 0
                    pause()

        elif settings[15]==0:
            info = [(random.randint(50,255),random.randint(50,255),random.randint(50,255)),
                mousepos[0],mousepos[1],
                random.randint(settings[2],settings[3]),
                0,0,
                random.randint(-settings[4],settings[4]),random.randint(-settings[4],settings[4])]
            planetarray.append(info)
            
    if event.type == KEYDOWN:
        print(event)
        if (event.key == 32):
            pause()
        if (event.key == 114):
            restart()
    return settings

def controlsdraw():
    global settings
    global offset
    global controlRects
    pause()
    pygame.draw.rect(DISPLAYSURF,(50,50,150),(textRectObj.left,0,(screenw-textRectObj.left),(screenh)))
    controlRects = []
    for n in range(0,len(labelsurfs)):
        controlRects.append(pygame.draw.rect(DISPLAYSURF,(255,255,255),
                                             (textRectObj.left+5,textRectObj.bottom+offset,(screenw-(textRectObj.left+10)),20)))
        DISPLAYSURF.blit(labelsurfs[n],labelrects[n])
        offset += 40
    offset = 20
    linenum = -1
    for rect in controlRects:
        linenum+=1
        if namelist[linenum] == 'FPS':
            pygame.draw.line(DISPLAYSURF, (0,0,0), ((rect.left + (settings[0]*rect.width/60)), rect.top),
                ((rect.left + (settings[0]*rect.width/60)), rect.bottom), 3)
        elif namelist[linenum] == 'Number of Planets':
            pygame.draw.line(DISPLAYSURF, (0,0,0), ((rect.left + (settings[1]*rect.width/500)), rect.top),
                ((rect.left + (settings[1]*rect.width/500)), rect.bottom), 3)
        elif namelist[linenum] == 'Radius of Planets':
            pygame.draw.line(DISPLAYSURF, (0,0,0), ((rect.left + (settings[3]*rect.width/30))-2, rect.top),
                ((rect.left + (settings[3]*rect.width/30))-2, rect.bottom),3)
        elif namelist[linenum] == 'Initial Speed':
            pygame.draw.line(DISPLAYSURF, (0,0,0), ((rect.left + (settings[4]*rect.width/10)), rect.top),
                ((rect.left + (settings[4]*rect.width/10)), rect.bottom), 3)
        elif namelist[linenum] == 'Trail Length':
            pygame.draw.line(DISPLAYSURF, (0,0,0), ((rect.right - (settings[5]*rect.width/255)), rect.top),
                ((rect.right - (settings[5]*rect.width/255)), rect.bottom), 3)
        elif namelist[linenum] == 'Friction':
            pygame.draw.line(DISPLAYSURF, (0,0,0), ((rect.left + (settings[16]*rect.width/100)), rect.top),
                ((rect.left + (settings[16]*rect.width/100)), rect.bottom), 3)
        elif namelist[linenum] == 'Absorbing Collisions':
            if(settings[12]):
                pygame.draw.rect(DISPLAYSURF,(50,200,100),(rect.left,rect.top,rect.width,rect.height))
            elif(settings[12]==0):
                pygame.draw.rect(DISPLAYSURF,(200,50,50),(rect.left,rect.top,rect.width,rect.height))
        elif namelist[linenum] == 'Border Bouncing':
            if(settings[11]):
                pygame.draw.rect(DISPLAYSURF,(50,200,100),(rect.left,rect.top,rect.width,rect.height))
            elif(settings[11]==0):
                pygame.draw.rect(DISPLAYSURF,(200,50,50),(rect.left,rect.top,rect.width,rect.height))
        elif namelist[linenum] == 'Gravity':
            if(settings[17]):
                pygame.draw.rect(DISPLAYSURF,(50,200,100),(rect.left,rect.top,rect.width,rect.height))
            elif(settings[17]==0):
                pygame.draw.rect(DISPLAYSURF,(200,50,50),(rect.left,rect.top,rect.width,rect.height))
        elif namelist[linenum] == 'Billiard Collisions':
            if(settings[18]):
                pygame.draw.rect(DISPLAYSURF,(50,200,100),(rect.left,rect.top,rect.width,rect.height))
            elif(settings[18]==0):
                pygame.draw.rect(DISPLAYSURF,(200,50,50),(rect.left,rect.top,rect.width,rect.height))
    linenum = 0
    settings[15] = 1


def pause():
    global settings
    if settings[14] == 1:
        settings[14] = 0
    elif settings[14] == 0:
        settings[14] = 1
        pygame.draw.rect(DISPLAYSURF,(255,255,255),((screenw/2)-15,(screenh/2)-20,10,40))
        pygame.draw.rect(DISPLAYSURF,(255,255,255),((screenw/2)+15,(screenh/2)-20,10,40))

#run the game loop
planetarray = planetcreate(nplanets)
while True:
    for event in pygame.event.get():
        eventhandle(event)
        
    if settings[14] == 0:
        planetarray = gravitycalc(planetarray)
        planetdraw(planetarray)
    DISPLAYSURF.blit(textSurfaceObj, textRectObj)

    
    if(len(planetarray) == 1):
        if frames == 3:
            restart()
            frames = 0
        frames += 1
    pygame.display.update()
    fpsClock.tick(settings[0])


