object ConfigDataModule: TConfigDataModule
  OldCreateOrder = False
  Left = 527
  Top = 345
  Height = 298
  Width = 839
  object deleteSQL: TQuery
    SQL.Strings = (
      'delete from config where entry_name = :theName')
    Left = 40
    Top = 8
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'theName'
        ParamType = ptUnknown
      end>
  end
  object selectStringSQL: TQuery
    SQL.Strings = (
      'select string_value from config where entry_name = :theName')
    Left = 40
    Top = 56
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'theName'
        ParamType = ptUnknown
      end>
  end
  object selectIntSQL: TQuery
    SQL.Strings = (
      'select int_value from config where entry_name = :theName')
    Left = 40
    Top = 120
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'theName'
        ParamType = ptUnknown
      end>
  end
  object updateStringSQL: TQuery
    SQL.Strings = (
      'update config'
      'set string_value = :theString'
      'where entry_name = :theName')
    Left = 128
    Top = 56
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'theString'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'theName'
        ParamType = ptUnknown
      end>
  end
  object insertStringSQL: TQuery
    SQL.Strings = (
      'insert into config'
      '( entry_name, string_value )'
      'values'
      '( :entry_name, :string_value )')
    Left = 208
    Top = 56
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'entry_name'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'string_value'
        ParamType = ptUnknown
      end>
  end
  object updateIntSQL: TQuery
    SQL.Strings = (
      'update config'
      'set int_value = :theInteger'
      'where entry_name = :theName')
    Left = 128
    Top = 120
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'theInteger'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'theName'
        ParamType = ptUnknown
      end>
  end
  object insertIntSQL: TQuery
    SQL.Strings = (
      'insert into config'
      '( entry_name, int_value)'
      'values'
      '( :entry_name, :int_value )')
    Left = 208
    Top = 120
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'entry_name'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'int_value'
        ParamType = ptUnknown
      end>
  end
end
