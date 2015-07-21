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

        foreach (new DirectoryIterator($this->path) as $fileInfo)
        {
            if($fileInfo->isDot()) continue;
            if( $fileInfo->isDir() )
            {
                $idList[] = $fileInfo->getFilename();
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

        foreach (new DirectoryIterator($this->path) as $fileInfo)
        {
            if($fileInfo->isDot()) continue;
            if( $fileInfo->isDir() )
            {
                $idList[] = $fileInfo->getFilename();
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

    public function __construct($path, $type)
    {
        parent::__construct();
        $this->path = $path;
        $this->type = $type;
    }

    public function getItem($id)
    {

        $this->itemID = $id;

        $itemPath = $this->path . '/' . $id;

        if(is_dir($itemPath))
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

    private function setItemObject($item)
    {

        $elementsArray = preg_split ('/\r\n|\n|\r/', $item, -1, PREG_SPLIT_NO_EMPTY | PREG_SPLIT_DELIM_CAPTURE);

        $counter = 0;
        if(!empty($elementsArray))
        {
            $objArray = array();

            $title = $elementsArray[0];
            $content = NULL;

            if(isset($elementsArray[1]))
            {
                $content = explode(',', $elementsArray[1]);
            }

            $objArray['title'] = $title;
            $objArray['content'] = $content;

            $this->item = isset($objArray) && !empty($objArray) ? (object)$objArray : NULL;

        }

    }

    private function getItemObject()
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