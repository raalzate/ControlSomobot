package com.espacionunido.controlsomobot

import android.annotation.SuppressLint
import android.view.MotionEvent
import android.view.View
import android.view.View.OnTouchListener
import android.widget.RelativeLayout

/**
 * Created by admin on 17/01/2017.
 */
class OnCursorListener(val mainLayout : RelativeLayout, val callback: (Int, Int) -> Unit) : OnTouchListener {
    private var xDelta: Int = 0
    private var yDelta: Int = 0

    @SuppressLint("ClickableViewAccessibility")
    override fun onTouch(view: View?, event: MotionEvent?): Boolean {

        when (event!!.action and MotionEvent.ACTION_MASK) {

            MotionEvent.ACTION_DOWN -> {
                val layoutParams = view!!.layoutParams as RelativeLayout.LayoutParams

                xDelta = (event.rawX - layoutParams.leftMargin).toInt()
                yDelta = (event.rawY - layoutParams.topMargin).toInt()
            }

            MotionEvent.ACTION_UP -> callback((event.rawX ).toInt(), (event.rawY ).toInt())

            MotionEvent.ACTION_MOVE -> {
                val layoutParams = view!!.layoutParams as RelativeLayout.LayoutParams
                layoutParams.leftMargin = (event.rawX - xDelta).toInt()
                layoutParams.topMargin = (event.rawY - yDelta).toInt()
                layoutParams.rightMargin = 0
                layoutParams.bottomMargin = 0
                view.layoutParams = layoutParams
            }
        }

        mainLayout.invalidate()
        return true
    }
}