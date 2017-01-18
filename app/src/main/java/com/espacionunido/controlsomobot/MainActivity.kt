package com.espacionunido.controlsomobot

import android.content.Context
import android.os.Bundle
import android.os.Vibrator
import android.support.v7.app.AppCompatActivity
import android.view.View
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
    private var  centerX = 0F
    private var centerY = 0F
    private var cursorH = 0
    private var  cursorW = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val viewCollisions = listOf(bgCenter, bgBottomLeft, bgBottomRight, bgTopRight, bgTopLeft, bgBottom, bgTop)

        layoutMain.post({
            cursorH = cursor.height
            cursorW = cursor.width
            centerX = ((layoutMain.width - cursorW ) / 2).toFloat()
            centerY = ((layoutMain.height - cursorH ) / 2).toFloat()
            centerCursor(centerX, centerY)
        })

        viewCollisions.forEach {
            it.setOnClickListener {
                adjustment(it)
            }
        }

        cursor.setOnTouchListener(OnCursorListener(layoutMain) { x: Int, y: Int ->
            printIndicator((x-cursor.width/2).toInt(), (y- cursor.height/2).toInt())
            viewCollisions.forEach {
                if(hasCollision(it)){
                    adjustment(it)
                }
            }
        })
    }

    private fun adjustment(view: View){
        val vibrator = getSystemService(Context.VIBRATOR_SERVICE) as Vibrator
        val x = view.x + (view.width - cursor.width)/2
        val y = view.y + (view.height - cursor.height)/2
        vibrator.vibrate(80)
        centerCursor(x, y)
        printIndicator(x.toInt(), y.toInt())
    }

    private fun  hasCollision(view: View):Boolean{

        val xDif = view.x - cursor.x
        val yDif = view.y - cursor.y
        val radiusSum = cursor.width/2 + view.width/2
        val distance = Math.sqrt(Math.pow(xDif.toDouble(), 2.0) + Math.pow(yDif.toDouble(), 2.0))

        return distance < radiusSum
    }

    private fun printIndicator(x:Int, y:Int){
        val d = centerX - 90
        indicator.text = "X=${x-d}, x=$x"
    }

    private fun centerCursor(x:Float, y: Float){
        cursor.x = x
        cursor.y = y
    }

}
