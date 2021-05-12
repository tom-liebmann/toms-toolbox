package toms_toolbox;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;
import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.function.Predicate;
import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

class MainView extends GLSurfaceView
{
    private static final String TAG = "MainView";

    private final Queue< MotionEvent > m_motionEvents = new LinkedList<>();
    private final Lock m_lock = new ReentrantLock();

    public MainView( Context context )
    {
        super( context );

        setPreserveEGLContextOnPause( true );

        setEGLContextFactory( new ContextFactory() );

        setEGLConfigChooser( 8, 8, 8, 8, 16, 0 );

        setRenderer( new Renderer() );
    }

    @Override
    public boolean onTouchEvent( MotionEvent event )
    {
        m_lock.lock();
        m_motionEvents.add( MotionEvent.obtain( event ) );
        m_lock.unlock();
        return true;
    }

    private void handleMotionEvent( MotionEvent event )
    {
        final int action = event.getActionMasked();

        switch( action )
        {
            case MotionEvent.ACTION_POINTER_DOWN:
            case MotionEvent.ACTION_DOWN:
            {
                final int pointerNr = event.getActionIndex();
                final int pointerId = event.getPointerId( pointerNr );
                final float x = event.getX( pointerNr );
                final float y = event.getY( pointerNr );
                ApplicationLib.on_pointer_down( pointerId, x, y );
                break;
            }

            case MotionEvent.ACTION_POINTER_UP:
            case MotionEvent.ACTION_UP:
            {
                final int pointerNr = event.getActionIndex();
                final int pointerId = event.getPointerId( pointerNr );
                final float x = event.getX( pointerNr );
                final float y = event.getY( pointerNr );
                ApplicationLib.on_pointer_up( pointerId, x, y );
                break;
            }

            case MotionEvent.ACTION_MOVE:
            {
                for( int pointerNr = 0; pointerNr < event.getPointerCount(); ++pointerNr )
                {
                    final int pointerId = event.getPointerId( pointerNr );
                    final float x = event.getX( pointerNr );
                    final float y = event.getY( pointerNr );
                    ApplicationLib.on_pointer_move( pointerId, x, y );
                }
                break;
            }
        }
    }

    private static class ContextFactory implements GLSurfaceView.EGLContextFactory
    {
        private static final int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

        public EGLContext createContext( EGL10 egl, EGLDisplay display, EGLConfig eglConfig )
        {
            // clang-format off
            final int[] attribList = {
                EGL_CONTEXT_CLIENT_VERSION, 3,
                EGL10.EGL_NONE };
            // clang-format on

            final EGLContext context =
                egl.eglCreateContext( display, eglConfig, EGL10.EGL_NO_CONTEXT, attribList );

            return context;
        }

        public void destroyContext( EGL10 egl, EGLDisplay display, EGLContext context )
        {
            egl.eglDestroyContext( display, context );
        }
    }

    private static void checkEglError( String prompt, EGL10 egl )
    {
        int error;
        while( ( error = egl.eglGetError() ) != EGL10.EGL_SUCCESS )
        {
            Log.e( TAG, String.format( "%s: EGL error: 0x%x", prompt, error ) );
        }
    }

    private class Renderer implements GLSurfaceView.Renderer
    {
        public void onDrawFrame( GL10 gl )
        {
            m_lock.lock();
            while( !m_motionEvents.isEmpty() )
            {
                final MotionEvent event = m_motionEvents.poll();

                if( event != null )
                {
                    handleMotionEvent( event );
                }
            }
            m_lock.unlock();

            ApplicationLib.step();
        }

        public void onSurfaceChanged( GL10 gl, int width, int height )
        {
            ApplicationLib.on_resize( width, height );
        }

        public void onSurfaceCreated( GL10 gl, EGLConfig config )
        {
            ApplicationLib.init_gl();
        }
    }
}
