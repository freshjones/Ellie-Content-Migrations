<?php 

class EllieContentMigrateList extends MigrateList
{

    private $path;
    private $type;

    public function __construct($list_url, $type = 'pages')
    {
        parent::__construct();
        $this->path = $list_url;
        $this->type = $type;
    }

    /**
     * Our public face is the dir we're getting items from
     *
     * @return string
     */
    public function __toString()
    {
        return $this->path;
    }

    /**
     * Load each directory within the content type, and return an array of the IDs for each directory.
     *
     * @return array
     */
    public function getIdList()
    {

        $idList = array();
        
        $directory = new RecursiveDirectoryIterator($this->path);
        $iterator = new RecursiveIteratorIterator($directory, RecursiveIteratorIterator::SELF_FIRST);
        
        foreach ($iterator as $fileInfo)
        {
            if( $fileInfo->isDir() )
            {
                $fullname = trim($fileInfo->getFilename());
        
                $delimiter = strpos($fullname, "_");
        
                $dirname = substr($fullname, 0, $delimiter);
        
                if($dirname === $this->type)
                {
                    $idList[] = $fullname;
                }
        
            }
        }

        if(!empty($idList))
        {
            return $idList;
        }

        Migration::displayMessage(t('Loading of !listurl failed:',
            array('!listurl' => $this->listUrl)));

        return null;

    }

    public function computeCount()
    {
        $count = 0;

        $idList = array();

        $directory = new RecursiveDirectoryIterator($this->path);
        $iterator = new RecursiveIteratorIterator($directory, RecursiveIteratorIterator::SELF_FIRST);
        
        foreach ($iterator as $fileInfo) 
        {
            if( $fileInfo->isDir() )
            {
                $fullname = trim($fileInfo->getFilename());
                
                $delimiter = strpos($fullname, "_");
                
                $dirname = substr($fullname, 0, $delimiter);
                
                if($dirname === $this->type)
                {
                    $idList[] = $fullname;
                }
                
            }
        }

        if(!empty($idList))
        {
            return count($idList);
        }

        return $count;

    }

}

class EllieContentMigrateItem extends MigrateItem
{

    protected $path;
    protected $type;
    protected $itemID = NULL;
    protected $item = NULL;
    protected $itemPath = NULL;
    
    public function __construct($path, $type)
    {
        parent::__construct();
        $this->path = $path;
        $this->type = $type;
    }
    
    public function getItem($id)
    {
        
        //set the itemID
        $this->itemID = $id;
        
        //map the correct path to the item
        $this->setItemPath();
        
        //get the path
        $itemPath = $this->getItemPath();

        if(!is_null($itemPath) && is_dir($itemPath))
        {

            $filename = $itemPath . '/content.txt';
   
            if(file_exists($filename))
            {
                $itemContent = file_get_contents($filename);

                if(strlen($itemContent))
                {
                    $this->setItemObject($itemContent);

                }

            }
             
        }

        return $this->getItemObject();

    }

    private function setItemPath()
    {
        
        $directory = new RecursiveDirectoryIterator($this->path);
        $iterator = new RecursiveIteratorIterator($directory, RecursiveIteratorIterator::SELF_FIRST);
        
        foreach ($iterator as $fileInfo)
        {
            if( $fileInfo->isDir() )
            {
                $dirname = trim($fileInfo->getFilename());
        
                if($dirname === $this->itemID)
                {
                    $this->itemPath = $fileInfo->getPathname();
                }
        
            }
        }
        
    }
    
    private function getItemPath()
    {
        return $this->itemPath;
    }
    
    protected function setItemObject($item)
    {

        $objArray = array();
        $objArray['text'] = $item;

        $this->item = isset($objArray) && !empty($objArray) ? (object)$objArray : NULL;

    }

    protected function getItemObject()
    {

        if(is_null($this->item) )
        {
            $migration = Migration::currentMigration();
            $message =  t('Loading of !objecturl failed:', array('!objecturl' => $this->path));
            $migration->getMap()->saveMessage(
                array($this->itemID), $message, MigrationBase::MESSAGE_ERROR);
            return NULL;
        }

        return $this->item;

    }

}