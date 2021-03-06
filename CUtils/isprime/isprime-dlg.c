#include <windows.h>
#include <stdlib.h>
#include "primesft.h"
#include "resource.h" 

BOOL CALLBACK DlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
/*
int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow){
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DlgProc);
}
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DlgProc);
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_INITDIALOG:
			// This is where we set up the dialog box, and initialise any default values

			SetDlgItemInt(hwnd, IDC_NUMBER, 30402457, FALSE);
		break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_ADDSIFT:
				{
					char numadd[11];
					int bSuccess = GetDlgItemText(hwnd, IDC_NUMBER, numadd, 11);
					if(bSuccess) 
					{
						SendDlgItemMessage(hwnd, IDC_SIFTLIST, LB_ADDSTRING, 0, (LPARAM)numadd);
						// Here we are associating the value nTimes with the item 
						// just for the heck of it, we'll use it to display later.
						// Normally you would put some more useful data here, such
						// as a pointer.
						//SendDlgItemMessage(hwnd, IDC_LIST, LB_SETITEMDATA, (WPARAM)index, (LPARAM)nTimes);
					}
				/*	else 
					{
						MessageBox(hwnd, "You didn't enter anything!", "Warning", MB_OK);
					}	*/
				}
				break;

				case IDC_REMOVESIFT:
				{
					// When the user clicks the Remove button, we first get the number
					// of selected items

					HWND hList = GetDlgItem(hwnd, IDC_SIFTLIST);
					int count = SendMessage(hList, LB_GETSELCOUNT, 0, 0);
					if(count != LB_ERR)
					{
						if(count != 0)
						{
							// And then allocate room to store the list of selected items.

							int i;
							int *buf = (int*)GlobalAlloc(GPTR, sizeof(int) * count);
							SendMessage(hList, LB_GETSELITEMS, (WPARAM)count, (LPARAM)buf);
							
							// Now we loop through the list and remove each item that was
							// selected.  

							// WARNING!!!  
							// We loop backwards, because if we removed items
							// from top to bottom, it would change the indexes of the other
							// items!!!

							for(i = count - 1; i >= 0; i--)
							{
								SendMessage(hList, LB_DELETESTRING, (WPARAM)buf[i], 0);
							}

							GlobalFree(buf);
						}
						else 
						{
							MessageBox(hwnd, "No items selected.", "Error", MB_OK);
						}
					}
					else
					{
						MessageBox(hwnd, "Error counting items.", "Warning", MB_OK);
					}
				}
				break;
		
				case IDC_CLEARSIFT:
					SendDlgItemMessage(hwnd, IDC_SIFTLIST, LB_RESETCONTENT, 0, 0);
				break;

				case IDC_REMOVEPLIST:
				{
					// When the user clicks the Remove button, we first get the number
					// of selected items

					HWND hList = GetDlgItem(hwnd, IDC_PRIMELIST);
					int count = SendMessage(hList, LB_GETSELCOUNT, 0, 0);
					if(count != LB_ERR)
					{
						if(count != 0)
						{
							// And then allocate room to store the list of selected items.

							int i;
							int *buf = (int*)GlobalAlloc(GPTR, sizeof(int) * count);
							SendMessage(hList, LB_GETSELITEMS, (WPARAM)count, (LPARAM)buf);
							
							// Now we loop through the list and remove each item that was
							// selected.  

							// WARNING!!!  
							// We loop backwards, because if we removed items
							// from top to bottom, it would change the indexes of the other
							// items!!!

							for(i = count - 1; i >= 0; i--)
							{
								SendMessage(hList, LB_DELETESTRING, (WPARAM)buf[i], 0);
							}

							GlobalFree(buf);
						}
						else 
						{
							MessageBox(hwnd, "No items selected.", "Error", MB_OK);
						}
					}
					else
					{
						MessageBox(hwnd, "Error counting items.", "Warning", MB_OK);
					}
				}
				break;
		
				case IDC_CLEARPLIST:
					SendDlgItemMessage(hwnd, IDC_PRIMELIST, LB_RESETCONTENT, 0, 0);
				break;

				case IDC_SIFT:
				{
					//Save the current cursor and set a busy cursor
					HCURSOR SavCrs = GetCursor();
					SetCursor(LoadCursor(0, (LPCTSTR)IDC_WAIT));

					HWND hList = GetDlgItem(hwnd, IDC_SIFTLIST);
					int count = SendMessage(hList, LB_GETCOUNT, 0, 0);
					if(count != LB_ERR)
					{
						if(count != 0)
						{
							int i, nums[count];
							for(i = 0; i < count; i++) //Copy the sift list into an array
							{
								char *buf = (char*)GlobalAlloc(GPTR, sizeof(char) * (int)SendMessage(hList, LB_GETTEXTLEN, (WPARAM)i, 0));
								SendMessage(hList, LB_GETTEXT, (WPARAM)i, (LPARAM)buf);
								nums[i] = atoi(buf);
								GlobalFree(buf);
							}
							SiftPrimes(nums, count, 0); //Sift!

							char numadd[11];
							for(i = 0; i < count; i++) //Put the resulting primes into the prime list...
							{
								if(nums[i] != 0) {
									itoa(nums[i], numadd, 10);
									SendDlgItemMessage(hwnd, IDC_PRIMELIST, LB_ADDSTRING, 0, (LPARAM)numadd);
								}
							}
							SendDlgItemMessage(hwnd, IDC_PRIMELIST, LB_ADDSTRING, 0, (LPARAM)""); //...and add a blank line.
						}
						else 
						{
							MessageBox(hwnd, "No items in sift list!", "Error", MB_OK);
						}
					}
					else
					{
						MessageBox(hwnd, "Error counting items.", "Warning", MB_OK);
					}

					//Restore cursor
					SetCursor(SavCrs);
				}
				break;

			/* 	case IDC_SIFTLIST:
					switch(HIWORD(wParam))
					{
						case LBN_SELCHANGE:
						{
							// Get the number of items selected.

							HWND hList = GetDlgItem(hwnd, IDC_LIST);
							int count = SendMessage(hList, LB_GETSELCOUNT, 0, 0);
							if(count != LB_ERR)
							{
								// We only want to continue if one and only one item is
								// selected.

								if(count == 1)
								{
									// Since we know ahead of time we're only getting one
									// index, there's no need to allocate an array.

									int index;
									int err = SendMessage(hList, LB_GETSELITEMS, (WPARAM)1, (LPARAM)&index);
									if(err != LB_ERR)
									{
										// Get the data we associated with the item above
										// (the number of times it was added)

										int data = SendMessage(hList, LB_GETITEMDATA, (WPARAM)index, 0);

										SetDlgItemInt(hwnd, IDC_SHOWCOUNT, data, FALSE);
									}
									else 
									{
										MessageBox(hwnd, "Error getting selected item :(", "Warning", MB_OK);
									}
								}
								else 
								{
									// No items selected, or more than one
									// Either way, we aren't going to process this.
									SetDlgItemText(hwnd, IDC_SHOWCOUNT, "-");
								}
							}
							else
							{
								MessageBox(hwnd, "Error counting items :(", "Warning", MB_OK);
							}
						}
						break;
					}
				break;
				*/
			}
			break;

		case WM_CLOSE:
			EndDialog(hwnd, 0);
			break;

		default:
			return FALSE;
	}
	return TRUE;
}
