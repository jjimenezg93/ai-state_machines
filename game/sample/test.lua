MOAISim.openWindow("game", 1024, 768)

viewport = MOAIViewport.new()
viewport:setSize (1024, 768)
viewport:setScale (1024, -768)

layer = MOAILayer2D.new()
layer:setViewport(viewport)
MOAISim.pushRenderPass(layer)

--Different textures
tex = "dragon_idle.png"
idleGfxQuad = MOAIGfxQuad2D.new()
idleGfxQuad:setTexture(tex)
idleGfxQuad:setRect(-32, -32, 32, 32)
idleGfxQuad:setUVRect(0, 0, 1, 1)
tex2 = "dragon_alarm.png"
alarmGfxQuad = MOAIGfxQuad2D.new()
alarmGfxQuad:setTexture(tex2)
alarmGfxQuad:setRect(-32, -32, 32, 32)
alarmGfxQuad:setUVRect(0, 0, 1, 1)
tex3 = "dragon_windup.png"
windupGfxQuad = MOAIGfxQuad2D.new()
windupGfxQuad:setTexture(tex3)
windupGfxQuad:setRect(-32, -32, 32, 32)
windupGfxQuad:setUVRect(0, 0, 1, 1)
  
prop = MOAIProp2D.new()
prop:setDeck(idleGfxQuad)

entity = Character.new()
-- Add prop to be the renderable for this character
entity:setProp(prop, layer)
-- Start the character (allow calls to OnUpdate)
entity:start()
entity:setLoc(0, 0)
entity:setRot(0)
--entity:setLinearVel(10, 20)
--entity:setAngularVel(30)

entity:addImage(idleGfxQuad)
entity:addImage(alarmGfxQuad)
entity:addImage(windupGfxQuad)

-- Enable Debug Draw
debug = MOAIDrawDebug.get();
layer:setDrawDebug(debug)
-- Add this character to draw debug
MOAIDrawDebug.insertEntity(entity)

mouseX = 0
mouseY = 0

function onClick(down)
	entity:setLoc(mouseX, mouseY)
end

function onRightClick(down)
	entity:setTarget(mouseX, mouseY)
	entity:setTargetActive()
end

function pointerCallback(x, y)
    mouseX, mouseY = layer:wndToWorld(x, y)
end

MOAIInputMgr.device.mouseLeft:setCallback(onClick)
MOAIInputMgr.device.mouseRight:setCallback(onRightClick)
MOAIInputMgr.device.pointer:setCallback(pointerCallback)
