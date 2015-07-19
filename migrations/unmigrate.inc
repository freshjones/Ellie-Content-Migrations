<?php


class buildUnMigrateList
{

    private $list;
    private $defaultList;

    public function __construct()
    {
        $this->list = array();
        $this->loadDefaultList();
    }

    private function loadDefaultList()
    {
        $defaults = array('created', 'changed', 'promote', 'sticky', 'revision', 'log', 'language', 'tnid', 'translate', 'revision_uid', 'is_new', 'comment');
        $this->defaultList = $defaults;
    }

    private function sanitizeList($var)
    {
        return array_unique($var);
    }

    public function setDefaults($var)
    {
        $this->defaultList = array();

        if(is_string($var))
        {
            $var = (array)$var;
        }
        if(!empty($var))
        {
            foreach($var AS $eachVar)
            {
                $this->defaultList[] = $eachVar;
            }
        }
    }

    public function setListItems($var)
    {
        if(is_string($var))
        {
            $var = (array)$var;
        }

        foreach($var AS $eachVar)
        {
            $this->list[] = $eachVar;
        }
    }

    public function getListItems()
    {
        $listItems = array_merge($this->defaultList, $this->list);
        return !empty($listItems) ? $this->sanitizeList($listItems) : array();
    }
    
}