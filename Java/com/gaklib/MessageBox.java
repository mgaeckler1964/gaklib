package com.gaklib;

import java.awt.*;
import java.awt.event.*;

/**
 * <p>Display a modal message box</p>
 * @version 1.0
 * @since 1.0
 * @author Martin Gaeckler - <a href="http://www.cresd.de">CRESD GmbH</a>
 */
public class MessageBox extends Dialog
{
	/**
	 * add an OK Button
	 * @see OK_LABEL
	 */
	public static final int OK_BUTTON = 1;
	/**
	 * add a cancel button
	 * @see CANCEL_LABEL
	 */
	public static final int CANCEL_BUTTON = 2;
	/**
	 * add a retry button
	 */
	public static final int RETRY_BUTTON = 4;
	/**
	 * add an ignore button
	 */
	public static final int IGNORE_BUTTON = 8;

	/**
	 * label for the OK button
	 */
	public static String OK_LABEL		= "      OK      ";
	/**
	 * label of the cancel button
	 */
	public static String CANCEL_LABEL	= "    Cancel    ";
	/**
	 * label of the retry button
	 */
	public static String RETRY_LABEL	= "     Retry    ";
	/**
	 * label of the ignore button
	 */
	public static String IGNORE_LABEL	= "    Ignore    ";

	static final int MAX_LINES = 32;
	static final int MAX_LINE_WIDTH = 60;

	GridBagLayout	dialogLayout		= new GridBagLayout();

	Panel			buttonPanel			= new Panel();
	GridBagLayout	buttonLayout		= new GridBagLayout();

	Panel			theMessagePanel		= new Panel();
	GridBagLayout	messageLayout		= new GridBagLayout();

	Label			theMessageLabels[]	= null;
	Button			okButton			= null;
	Button			cancelButton		= null;
	Button			retryButton			= null;
	Button			ignoreButton		= null;

	int				returnButton		= 0;

	/**
	 * displays a modal message box
	 * @param title String		- the title of the form
	 * @param message String	- the message
	 * @param buttons int		- the buttons to include
	 * @param font String		- the font face
	 * @param decoration int	- the font decoration
	 * @param size int			- the font size
	 */
	public MessageBox( 	String title, String message, int buttons,
						String font, int decoration, int size )
	{
		super(new Frame(), title, true);

		Dimension			theSize;

		enableEvents(AWTEvent.WINDOW_EVENT_MASK);
		try
		{
			setLayout( dialogLayout );
			init(message, buttons, font, decoration, size );
			pack();
			setResizable( false );

			theSize = getSize();
			setLocation( 400-theSize.width/2, 300-theSize.height/2 );
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
		}
	}
	/**
	 * displays a message box
	 * @param title String		- the title of the form
	 * @param message String	- the message displayed
	 * @param buttons int		- the buttons to include
	 */
	public MessageBox( String title, String message, int buttons )
	{
		this(title, message, buttons, "Arial", Font.PLAIN, 12 );
	}

	private static String getExceptionMessage( Throwable exception )
	{
		String message = exception.getLocalizedMessage();
		if( message == null )
		{
			message = exception.getClass().getName();
		}
		StackTraceElement stack[] = exception.getStackTrace();
		for( StackTraceElement trace : stack )
		{
			message += '\n';
			message += trace.getClassName() + ' ' + trace.getMethodName() + ' ' + trace.getFileName() + ' ' + trace.getLineNumber();
		}
		return message;
	}
	public MessageBox( String title, Throwable exception, int buttons )
	{
		this(title, MessageBox.getExceptionMessage( exception ), buttons, "Arial", Font.PLAIN, 12 );
	}

	void init(	String iMessage, int buttons,
				String font, int decoration, int size ) throws Exception
	{
		int					numLines, i, pos;
		int					numButtons = 0;
		String				newMessage;
		GridBagConstraints	gbc;

		buttonPanel.setLayout(buttonLayout);
		gbc = new GridBagConstraints();
		gbc.gridy = 0;
		gbc.gridwidth = 1;
		gbc.gridheight = 1;
		gbc.insets = new Insets(5, 5, 5, 5);

		if( buttons == 0 )
			buttons = OK_BUTTON;

		if( (buttons & OK_BUTTON) != 0 )
		{
			okButton = new Button( OK_LABEL );
			okButton.addActionListener(new java.awt.event.ActionListener()
			{
				public void actionPerformed(ActionEvent e)
				{
					returnButton = OK_BUTTON;
					dispose();
				}
			});

			gbc.gridx = numButtons;
			buttonPanel.add(okButton, gbc);
			numButtons++;
		}

		if( (buttons & CANCEL_BUTTON) != 0 )
		{
			cancelButton = new Button( CANCEL_LABEL );
			cancelButton.addActionListener(new java.awt.event.ActionListener()
			{
				public void actionPerformed(ActionEvent e)
				{
					returnButton = CANCEL_BUTTON;
					dispose();
				}
			});
			gbc.gridx = numButtons;
			buttonPanel.add(cancelButton, gbc);
			numButtons++;
		}

		if( (buttons & RETRY_BUTTON) != 0 )
		{
			retryButton = new Button( RETRY_LABEL );
			retryButton.addActionListener(new java.awt.event.ActionListener()
			{
				public void actionPerformed(ActionEvent e)
				{
					returnButton = RETRY_BUTTON;
					dispose();
				}
			});
			gbc.gridx = numButtons;
			buttonPanel.add(retryButton, gbc);
			numButtons++;
		}

		if( (buttons & IGNORE_BUTTON) != 0 )
		{
			ignoreButton = new Button( IGNORE_LABEL );
			ignoreButton.addActionListener(new java.awt.event.ActionListener()
			{
				public void actionPerformed(ActionEvent e)
				{
					returnButton = IGNORE_BUTTON;
					dispose();
				}
			});
			gbc.gridx = numButtons;
			buttonPanel.add(ignoreButton, gbc);
			numButtons++;
		}

		theMessagePanel.setLayout(messageLayout);

		theMessageLabels = new Label[MAX_LINES];
		numLines = 0;
		pos = 0;
		newMessage = "";

		gbc = new GridBagConstraints();
		gbc.gridx = 0;
		gbc.gridwidth = 1;
		gbc.gridheight = 1;
		gbc.ipady = -(size/2);
		gbc.anchor = GridBagConstraints.NORTHWEST;

		for( i=0; i<=iMessage.length() && numLines < MAX_LINES; i++ )
		{
			if( i==iMessage.length() || iMessage.charAt( i ) == '\n' || (pos>MAX_LINE_WIDTH && iMessage.charAt( i ) == ' '))
			{
				theMessageLabels[numLines] = new Label( newMessage );
				theMessageLabels[numLines].setFont(new Font(font, decoration, size));

				gbc.gridy = numLines;
				theMessagePanel.add(theMessageLabels[numLines], gbc);

				numLines++;
				pos = 0;
				newMessage = "";
			}
			else
			{
				newMessage += iMessage.charAt( i );
				pos++;
			}
		}

		gbc = new GridBagConstraints();
		gbc.gridx = 0;
		gbc.gridy = 0;
		gbc.gridheight = 1;
		gbc.anchor = GridBagConstraints.CENTER;
		gbc.insets = new Insets(5, 5, 5, 5);

		add( theMessagePanel, gbc );

		gbc.gridy = 1;
		add( buttonPanel, gbc );
	}

	/**
	 * processes all window events and closes the form if required
	 * @param e WindowEvent
	 */
	protected void processWindowEvent(WindowEvent e)
	{
		if(e.getID() == WindowEvent.WINDOW_CLOSING)
		{
			dispose();
		}
		super.processWindowEvent(e);
	}

	/**
	 * Returns the button selected by the user
	 * @return int
	 */
	public int getReturnButton()
	{
		return returnButton;
	}

	private void jbInit() throws Exception
	{
	}
}
