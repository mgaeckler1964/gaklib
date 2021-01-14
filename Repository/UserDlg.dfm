object UserDialog: TUserDialog
  Left = 473
  Top = 177
  ActiveControl = Panel1
  BorderStyle = bsDialog
  Caption = 'User & Groups'
  ClientHeight = 313
  ClientWidth = 771
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 771
    Height = 41
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    object DBNavigatorUser: TDBNavigator
      Left = 8
      Top = 8
      Width = 240
      Height = 25
      DataSource = DataSourceUser
      Ctl3D = False
      ParentCtl3D = False
      TabOrder = 0
    end
    object BitBtnUsers: TBitBtn
      Left = 256
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Users'
      TabOrder = 1
      OnClick = BitBtnUsersClick
    end
    object BitBtnGroups: TBitBtn
      Left = 336
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Groups'
      TabOrder = 2
      OnClick = BitBtnGroupsClick
    end
  end
  object Panel2: TPanel
    Left = 321
    Top = 41
    Width = 450
    Height = 272
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 4
    TabOrder = 1
    object Label1: TLabel
      Left = 8
      Top = 40
      Width = 55
      Height = 13
      Caption = 'Department'
    end
    object Label2: TLabel
      Left = 8
      Top = 8
      Width = 28
      Height = 13
      Caption = 'Name'
    end
    object DBLookupComboBoxDepartment: TDBLookupComboBox
      Left = 96
      Top = 40
      Width = 321
      Height = 21
      DataField = 'DEPARTMENT'
      DataSource = DataSourceUser
      KeyField = 'id'
      ListField = 'username'
      ListSource = DataSourceGroups
      TabOrder = 1
    end
    object DBEditUsername: TDBEdit
      Left = 96
      Top = 8
      Width = 321
      Height = 21
      DataField = 'USERNAME'
      DataSource = DataSourceUser
      TabOrder = 0
    end
    object PageControl: TPageControl
      Left = 8
      Top = 72
      Width = 433
      Height = 193
      ActivePage = TabSheetUser
      TabOrder = 2
      object TabSheetUser: TTabSheet
        Caption = 'User'
        object Label3: TLabel
          Left = 8
          Top = 8
          Width = 51
          Height = 13
          Caption = 'Last Name'
        end
        object Label4: TLabel
          Left = 8
          Top = 32
          Width = 50
          Height = 13
          Caption = 'First Name'
        end
        object DBEditLastName: TDBEdit
          Left = 88
          Top = 8
          Width = 321
          Height = 21
          DataField = 'LASTNAME'
          DataSource = DataSourceUser
          TabOrder = 0
        end
        object GroupBoxPermissions: TGroupBox
          Left = 8
          Top = 68
          Width = 401
          Height = 89
          Caption = 'Permissions'
          TabOrder = 2
          object CheckBoxLogin: TCheckBox
            Left = 72
            Top = 16
            Width = 97
            Height = 17
            Caption = 'Login Enabled'
            TabOrder = 0
            OnClick = StartEditMode
          end
          object CheckBoxGroupAdmin: TCheckBox
            Left = 72
            Top = 32
            Width = 201
            Height = 17
            Caption = 'Group Administration'
            TabOrder = 1
            OnClick = StartEditMode
          end
          object CheckBoxUserAdmin: TCheckBox
            Left = 72
            Top = 48
            Width = 201
            Height = 17
            Caption = 'User Administration'
            TabOrder = 2
            OnClick = StartEditMode
          end
          object CheckBoxSystemAdmin: TCheckBox
            Left = 72
            Top = 64
            Width = 225
            Height = 17
            Caption = 'System Administration'
            TabOrder = 3
            OnClick = StartEditMode
          end
        end
        object DBEditFirstName: TDBEdit
          Left = 88
          Top = 32
          Width = 321
          Height = 21
          DataField = 'FIRSTNAME'
          DataSource = DataSourceUser
          TabOrder = 1
        end
      end
      object TabSheetGroup: TTabSheet
        Caption = 'Group'
        ImageIndex = 1
        object DBGridMember: TDBGrid
          Left = 8
          Top = 40
          Width = 465
          Height = 129
          DataSource = DataSourceMember
          TabOrder = 0
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clWindowText
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
        end
        object DBNavigatorMember: TDBNavigator
          Left = 8
          Top = 8
          Width = 240
          Height = 25
          DataSource = DataSourceMember
          TabOrder = 1
        end
      end
    end
  end
  object DBGridUser: TDBGrid
    Left = 0
    Top = 41
    Width = 321
    Height = 272
    Align = alLeft
    DataSource = DataSourceUser
    TabOrder = 2
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    OnDblClick = DBGridUserDblClick
  end
  object DataSourceUser: TDataSource
    DataSet = TableUser
    Left = 143
    Top = 93
  end
  object TableUser: TTable
    AfterInsert = TableUserAfterInsert
    BeforePost = TableUserBeforePost
    AfterPost = TableUserAfterPost
    BeforeDelete = TableUserBeforeDelete
    AfterScroll = TableUserAfterScroll
    DatabaseName = 'docManDB'
    IndexName = 'USERNAME_IDX'
    TableName = 'user_tab'
    Left = 51
    Top = 93
    object TableUserID: TIntegerField
      FieldName = 'ID'
      Visible = False
    end
    object TableUserTYPE: TIntegerField
      FieldName = 'TYPE'
      Required = True
      Visible = False
    end
    object TableUserMD5_PASSWD: TStringField
      FieldName = 'MD5_PASSWD'
      Visible = False
      Size = 32
    end
    object TableUserLASTNAME: TStringField
      FieldName = 'LASTNAME'
      Visible = False
      Size = 255
    end
    object TableUserFIRSTNAME: TStringField
      FieldName = 'FIRSTNAME'
      Visible = False
      Size = 255
    end
    object TableUserDEPARTMENT: TIntegerField
      FieldName = 'DEPARTMENT'
      Visible = False
    end
    object TableUserPERMISSIONS: TIntegerField
      FieldName = 'PERMISSIONS'
      Visible = False
    end
    object TableUserUSERNAME: TStringField
      DisplayLabel = 'Name'
      FieldName = 'USERNAME'
      Required = True
      Size = 64
    end
  end
  object DataSourceGroups: TDataSource
    DataSet = QueryGroups
    Left = 144
    Top = 168
  end
  object TableMember: TTable
    DatabaseName = 'docManDB'
    IndexFieldNames = 'GROUP_ID'
    MasterFields = 'ID'
    MasterSource = DataSourceUser
    TableName = 'U_MEMBER'
    Left = 61
    Top = 273
    object TableMemberGROUP_ID: TIntegerField
      FieldName = 'GROUP_ID'
      Visible = False
    end
    object TableMemberMEMBER_ID: TIntegerField
      FieldName = 'MEMBER_ID'
      LookupResultField = 'id'
      Visible = False
    end
    object TableMemberMEMBERNAME: TStringField
      DisplayLabel = 'Member'
      DisplayWidth = 60
      FieldKind = fkLookup
      FieldName = 'MEMBERNAME'
      LookupDataSet = QueryAll
      LookupKeyFields = 'id'
      LookupResultField = 'username'
      KeyFields = 'MEMBER_ID'
      Lookup = True
    end
  end
  object QueryGroups: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select id, username from user_tab where type=1 order by username')
    Left = 56
    Top = 168
  end
  object QueryAll: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select id, username from user_tab order by username')
    Left = 56
    Top = 224
  end
  object DataSourceMember: TDataSource
    DataSet = TableMember
    Left = 133
    Top = 289
  end
end
