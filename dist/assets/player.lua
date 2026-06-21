-- Script do player
speed = 150

function update(dt)
    x = getPositionX()
    y = getPositionY()

    -- movimento circular automatico como demonstracao
    time = (time or 0) + dt
    setPositionX(400 + math.cos(time * speed * 0.01) * 100)
    setPositionY(300 + math.sin(time * speed * 0.01) * 100)
end