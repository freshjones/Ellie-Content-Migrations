<?php

class EllieContentListJSON extends MigrateListJSON 
{
  protected function getIDsFromJSON(array $data)
  {
    $ids = array();
    foreach ($data as $item) 
    {
      $ids[] = $item['id'];
    }
    return $ids;
  }
}

class EllieContentItemJSON extends MigrateItemJSON {
  protected $data = array();

  public function getItem($id) {

    // We cache the parsed JSON at $this->data.
    if (empty($this->data)) {
      $data = $this->loadJSONUrl($this->itemUrl);
      if ($data) {
        // Let's index the array by the ID for easy retrieval.
        foreach ($data as $item) {
          $this->data[$item->id] = $item;
        }
      }
      else {
        // Error-handling here....
      }
    }
    // Return the requested item
    if (isset($this->data[$id])) {
      return $this->data[$id];
    }
    else {
      return NULL;
    }
  }
}

class PagesMigration extends Migration {

	public function __construct($arguments)
	{
		 
		parent::__construct($arguments);

		$this->description = t('Page Migrations');

		$this->map = new MigrateSQLMap($this->machineName,
	        array(
	          'pageID' => array('type' => 'int',
                                'unsigned' => TRUE,
                                'not null' => TRUE,
                               ),
	        ),
        	MigrateDestinationNode::getKeySchema()
      	);

		$fields = array(
		      'id' => 'Unique page ID',
		      'title' => 'Page title',
		      'description' => 'Page body',
		      'createdDate' => 'Page creation date',
		    );
		
		$json_file = __DIR__ . '/../data/data.json';
		
		$this->source = new MigrateSourceList(new EllieContentListJSON($json_file),
		
		new EllieContentItemJSON($json_file, array()), $fields);

		$this->destination = new MigrateDestinationNode('pages');
        
	}

}
